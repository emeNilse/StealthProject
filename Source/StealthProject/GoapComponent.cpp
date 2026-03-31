
#include "GoapComponent.h"
#include "Navigation/PathFollowingComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Math/UnrealMathUtility.h"
#include "AI_Controller.h"
#include "ActionPlan.h"

UGoapComponent::UGoapComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UGoapComponent::BeginPlay()
{
	Super::BeginPlay();
	CachedOwner = GetOwner();
	
	APawn* OwnerPawn = Cast<APawn>(CachedOwner);
	AI = Cast<AAI_Controller>(OwnerPawn->GetController());

	NPC = Cast<ANPC>(AI ? AI->GetPawn() : nullptr);
	AI_BlackBoard = AI->GetBlackboardComponent();

	WorldState = CachedOwner->FindComponentByClass<UGoapWorldStateComponent>();
	if (!IsValid(WorldState))
	{
		UE_LOG(LogTemp, Error, TEXT("GoapComp: world state comp missing on %s"), *CachedOwner->GetName());
		return;
	}

	if (UWorldFactRegistry* Registry = GetWorld()->GetGameInstance()->GetSubsystem<UWorldFactRegistry>())
	{
		for (const TWeakObjectPtr<AActor>& Provider : Registry->GetProviders())
		{
			if (Provider.IsValid())
			{
				WorldState->RegisterProvider(Provider.Get());
			}
		}
	}

	ActionStackComponent = CachedOwner->FindComponentByClass<UActionStackComponent>();
	ActionStackComponent->OnStackFailed.AddUObject(this, &UGoapComponent::HandlePlanFailed);
	ActionStackComponent->OnStackFinished.AddUObject(this, &UGoapComponent::HandlePlanFinished);

	SquadComponent = CachedOwner->FindComponentByClass<USquadComponent>();

	MyBeliefRegistry = MakeUnique<AgentBeliefs::BeliefRegistry>();
	MyBeliefFactory = MakeUnique<BeliefFactory>(this, *MyBeliefRegistry);

	if (GetWorld())
	{
		GoapFactory = GetWorld()->GetGameInstance()->GetSubsystem<UGoapFactorySubsystem>();

		if (IsValid(GoapFactory))
		{
			GoapPlanner = GoapFactory->CreatePlanner();
			UE_LOG(LogTemp, Warning, TEXT("%s assigned GoapPlanner at %p"), *CachedOwner->GetName(), GoapPlanner.Get());
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("GoapFactorySubsystem is missing"));
		}
	}

	SetupBeliefs();
	SetupAction();
	SetupGoals();
	MakeAPlanForActionStack();
}

void UGoapComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!AI_BlackBoard)
	{
		AI_BlackBoard = AI ? AI->GetBlackboardComponent() : nullptr;
	}

	if (!TheActionPlan.IsValid() || HasNPCStateChanged() || HasSquadStateChanged() || bShouldReplan)
	{
		if (ActionStackComponent->IsExecuting())
		{
			ActionStackComponent->AbortCurrentAction();
		}
		else
		{
			bShouldReplan = false;
			MakeAPlanForActionStack();
		}
	}
	
	//Debugging and Goap Display purposes
	if (CurrentGoal.IsValid() && ActionStackComponent->GetCurrentAction())
	{
		CurrentGoalText = "Goal: " + CurrentGoal->Name;
		CurrentActionText = "Action: " + ActionStackComponent->GetCurrentAction()->Name;
		CurrentSquadRoleText = "Role: " + UEnum::GetDisplayValueAsText(SquadComponent->SquadRole).ToString();
	} 
	else
	{
		CurrentGoalText = "Goal: ";
		CurrentActionText = "Action: ";
		CurrentSquadRoleText = "Role: No Role Assigned";
	}
}

void UGoapComponent::MakeAPlanForActionStack()
{
	WorldState->RebuildWorldFacts();
	CalculatePlan();

	if (TheActionPlan.IsValid() && TheActionPlan->AgentActions.Num() > 0)
	{
		CurrentGoal = TheActionPlan->AgentGoal;
		UE_LOG(LogTemp, Warning, TEXT("Goal: %s with %d actions in plan"), *CurrentGoal->Name, TheActionPlan->AgentActions.Num());
		//Algo::Reverse(TheActionPlan->AgentActions);
		for (TSharedPtr<GoapAction> ga : TheActionPlan->AgentActions)
		{
			ActionStackComponent->PushAction(ga);
			//UE_LOG(LogTemp, Warning, TEXT("Action: %s"), *ga->Name);
		}
	}
}

void UGoapComponent::HandlePlanFailed()
{
	LastGoal = CurrentGoal;
	TheActionPlan = nullptr;
	bShouldReplan = true;
}

void UGoapComponent::HandlePlanFinished()
{
	LastGoal = CurrentGoal;
	TheActionPlan = nullptr;
	bShouldReplan = true;
}

void UGoapComponent::SetupBeliefs()
{
	for (UGoapBelief* belief : BeliefSet)
	{
		if (belief)
		{
			MyBeliefFactory->AddBelief(belief->Name, [belief, this]() { return belief->Evaluate(AI); });
		}
	}
}

void UGoapComponent::SetupAction()
{
	for (UGoapActionsSet* action : ActionSets)
	{
		if (!action || !action->StrategyInstance)
		{
			continue;
		}

		//The runtime instance is all because of of the Dynamic Range Cost
		//If I ditch the dynamic range cost I will revert back to the previous Action setup method
		UGoapActionStrategyBase* RuntimeStrategy = action->StrategyInstance->CreateRunTimeInstance(this, AI);

		//Had to remove action->Cost from WithCost(), and replace it with the following lambda in order to 
		// make room for the Dynamic Range Cost that can be implemented in Move Strategies
		auto Builder = GoapAction::Builder(action->Name).WithStrategy(RuntimeStrategy).WithCost([RuntimeStrategy, this, action]() -> float { return RuntimeStrategy->GetCost(AI, action->Cost); });

		for (const FString& preconditionName : action->PreConditions)
		{
			Builder.AddPrecondition(MyBeliefRegistry->Get(preconditionName));
		}
		for (const FString& effectName : action->Effects)
		{
			Builder.AddEffect(MyBeliefRegistry->Get(effectName));
		}

		Actions.Add(Builder.Build());
	}
}

void UGoapComponent::SetupGoals()
{
	for (UGoapGoalObject* goal : GoalSet)
	{
		if (goal)
		{
			auto Builder = GoapGoal::Builder(goal->Name).WithPriority(goal->Priority);

			for (const FString& desiredEffectName : goal->DesiredEffects)
			{
				Builder.WithDesiredEffect(MyBeliefRegistry->Get(desiredEffectName));
			}

			Goals.Add(Builder.Build());
		}
	}
}

void UGoapComponent::CalculatePlan()
{
	int priorityLevel = CurrentGoal ? CurrentGoal->Priority : 0;

	TSet<TSharedPtr<GoapGoal>> goalsToCheck = Goals;

	if (CurrentGoal.IsValid())
	{
		//Current goal exists, checking goals with higher priority
		//Might need to tweak this. What if state change impacts the current goal?
		TSet<TSharedPtr<GoapGoal>> filteredGoals;

		for (TSharedPtr<GoapGoal> g : goalsToCheck)
		{
			if (g->Priority >= priorityLevel)
			{
				filteredGoals.Add(g);
			}
		}

		goalsToCheck = MoveTemp(filteredGoals);
	}

	TSharedPtr<ActionPlan> potentialPlan = GoapPlanner->Plan(this, AI, goalsToCheck, LastGoal);

	if (potentialPlan.IsValid())
	{
		TheActionPlan = potentialPlan;
	}
	else
	{
		TheActionPlan = nullptr;
		CurrentAction = nullptr;
		CurrentGoal = nullptr;
		//UE_LOG(LogTemp, Warning, TEXT("something strange with this place"));
	}
}


bool UGoapComponent::HasNPCStateChanged()
{
	if (LastNPCState != NPC->GetNPCState())
	{
		LastNPCState = NPC->GetNPCState();
		LastGoal = CurrentGoal;
		TheActionPlan = nullptr;
		return true;
	}

	return false;
}

bool UGoapComponent::HasSquadStateChanged()
{
	if (LastSquadState != SquadComponent->GetSquadState())
	{
		LastSquadState = SquadComponent->GetSquadState();
		LastGoal = CurrentGoal;
		TheActionPlan = nullptr;
		return true;
	}
	
	return false;
}

