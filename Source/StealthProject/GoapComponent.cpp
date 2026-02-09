// Fill out your copyright notice in the Description page of Project Settings.


#include "GoapComponent.h"
#include "Navigation/PathFollowingComponent.h"
#include "BeliefFactory.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Math/UnrealMathUtility.h"
#include "AI_Controller.h"
#include "ActionPlan.h"
#include "NPC.h"



UGoapComponent::UGoapComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UGoapComponent::BeginPlay()
{
	Super::BeginPlay();
	APawn* OwnerPawn = Cast<APawn>(GetOwner());
	AI = Cast<AAI_Controller>(OwnerPawn->GetController());
	NPC = Cast<ANPC>(AI ? AI->GetPawn() : nullptr);
	AI_BlackBoard = AI->GetBlackboardComponent();

	WorldState = GetOwner()->FindComponentByClass<UGoapWorldStateComponent>();
	if (!WorldState)
	{
		UE_LOG(LogTemp, Error, TEXT("GoapComp: world state comp missing on %s"), *GetOwner()->GetName());
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

	ActionStackComponent = GetOwner()->FindComponentByClass<UActionStackComponent>();
	ActionStackComponent->OnStackFailed.AddUObject(this, &UGoapComponent::HandlePlanFailed);
	ActionStackComponent->OnStackFinished.AddUObject(this, &UGoapComponent::HandlePlanFinished);

	Factory = MakeUnique<BeliefFactory>(this, Beliefs);

	if (GetWorld())
	{
		GoapFactory = GetWorld()->GetGameInstance()->GetSubsystem<UGoapFactorySubsystem>();

		if (GoapFactory)
		{
			GoapPlanner = GoapFactory->CreatePlanner();
			UE_LOG(LogTemp, Warning, TEXT("%s assigned GoapPlanner at %p"), *GetOwner()->GetName(), GoapPlanner.Get());
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

	if (!TheActionPlan.IsValid() || HasNPCStateChanged())
	{
		if (ActionStackComponent->IsExecuting())
		{
			ActionStackComponent->AbortCurrentAction();
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("new plan here"));
			MakeAPlanForActionStack();
		}
	}

	if (CurrentGoal.IsValid() && ActionStackComponent->GetCurrentAction())
	{
		CurrentGoalText = "Goal: " + CurrentGoal->Name;
		CurrentActionText = "Action: " + ActionStackComponent->GetCurrentAction()->Name;
	} 
	else
	{
		CurrentGoalText = "Goal: ";
		CurrentActionText = "Action: ";
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
		Algo::Reverse(TheActionPlan->AgentActions);
		for (TSharedPtr<GoapAction> ga : TheActionPlan->AgentActions)
		{
			ActionStackComponent->PushAction(ga);
		}
	}

	//UE_LOG(LogTemp, Warning, TEXT("plan updated?"));
	UpdateNPCState();
}

void UGoapComponent::HandlePlanFailed()
{
	//LastGoal, to do or not to do
	LastGoal = CurrentGoal;
	TheActionPlan = nullptr;
	MakeAPlanForActionStack();
}

void UGoapComponent::HandlePlanFinished()
{
	LastGoal = CurrentGoal;
	TheActionPlan = nullptr;
	MakeAPlanForActionStack();
}

void UGoapComponent::SetupBeliefs()
{
	for (UGoapBelief* belief : BeliefSet)
	{
		if (belief)
		{
			Factory->AddBelief(belief->Name, [belief, this]() { return belief->Evaluate(AI); });
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

		UGoapActionStrategyBase* RuntimeStrategy = action->StrategyInstance->CreateRunTimeInstance(this, AI);

		Actions.Add(
			GoapAction::Builder(action->Name)
			.WithStrategy(RuntimeStrategy)
			.WithCost(action->Cost)
			.AddPreconditions(action->PreConditions)
			.AddEffects(action->Effects)
			.Build());
	}
}

void UGoapComponent::SetupGoals()
{
	for (UGoapGoalObject* goal : GoalSet)
	{
		if (goal)
		{
			Goals.Add(GoapGoal::Builder(goal->Name).WithPriority(goal->Priority).WithDesiredEffect(goal->DesiredEffects).Build());
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
		UE_LOG(LogTemp, Warning, TEXT("something strange with this place"));
	}
}

void UGoapComponent::UpdateNPCState()
{
	LastNPCState.bCanSeePlayer = AI_BlackBoard->GetValueAsBool("bCanSeePlayer");
}

bool UGoapComponent::HasNPCStateChanged()
{
	if (LastNPCState.bCanSeePlayer != AI_BlackBoard->GetValueAsBool("bCanSeePlayer"))
	{
		bool cansee = AI_BlackBoard->GetValueAsBool("bCanSeePlayer");
		bool lastcansee = LastNPCState.bCanSeePlayer;
		UE_LOG(LogTemp, Warning, TEXT("bCanSeePlayer = %s"), cansee ? TEXT("true") : TEXT("false"));
		UE_LOG(LogTemp, Warning, TEXT("Last State bCanSeePlayer = %s"), lastcansee ? TEXT("true") : TEXT("false"));
		return true;
	}

	return false;
}

