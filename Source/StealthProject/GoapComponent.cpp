// Fill out your copyright notice in the Description page of Project Settings.


#include "GoapComponent.h"
#include "Navigation/PathFollowingComponent.h"
#include "BeliefFactory.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Math/UnrealMathUtility.h"
#include "AI_Controller.h"
#include "ActionPlan.h"
#include "NPC.h"



// Sets default values for this component's properties
UGoapComponent::UGoapComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UGoapComponent::BeginPlay()
{
	Super::BeginPlay();

	AI = Cast<AAI_Controller>(GetOwner()->GetInstigatorController());
	NPC = Cast<ANPC>(AI->GetPawn());
	AI_BlackBoard = AI->GetBlackboardComponent();

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


// Called every frame
void UGoapComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!TheActionPlan.IsValid() || HasNPCStateChanged())
	{
		if (ActionStackComponent->IsExecuting())
		{
			ActionStackComponent->AbortCurrentAction();
		}
		UE_LOG(LogTemp, Warning, TEXT("new plan here"));
		MakeAPlanForActionStack();
	}

	//if (!CurrentAction.IsValid() || HasNPCStateChanged())
	//{
	//	//Temp fix
	//	if (CurrentAction.IsValid() && HasNPCStateChanged())
	//	{
	//		CurrentAction->Stop();
	//	}
	//	//Need figure out why CalculatePlan() doesn't check if current action plan's preconditions are met
	//	CalculatePlan();

	//	if (TheActionPlan.IsValid() && TheActionPlan->AgentActions.Num() > 0)
	//	{
	//		CurrentGoal = TheActionPlan->AgentGoal;
	//		UE_LOG(LogTemp, Warning, TEXT("Goal: %s with %d actions in plan"), *CurrentGoal->Name, TheActionPlan->AgentActions.Num());
	//		CurrentAction = TheActionPlan->AgentActions[0];
	//		TheActionPlan->AgentActions.RemoveAt(0);
	//		UE_LOG(LogTemp, Warning, TEXT("Popped action: %s"), *CurrentAction->Name);

	//		//verify all precondition effects are true
	//		bool bAllPreconditionsMet = true;

	//		for (TSharedPtr<AgentBeliefs>& b : CurrentAction->Preconditions)
	//		{
	//			if (!b->Evaluate())
	//			{
	//				bAllPreconditionsMet = false;
	//				break;
	//			}
	//		}

	//		if (bAllPreconditionsMet)
	//		{
	//			CurrentAction->Start();
	//		}
	//		else
	//		{
	//			UE_LOG(LogTemp, Warning, TEXT("Preconditions not met"));
	//			CurrentAction = nullptr;
	//			CurrentGoal = nullptr;
	//		}
	//	}

	//	UE_LOG(LogTemp, Warning, TEXT("plan updated?"));
	//	UpdateNPCState();
	//}
	//
	////if there is a currentaction, execute
	//if (TheActionPlan.IsValid() && CurrentAction.IsValid())
	//{
	//	CurrentAction->Tick(DeltaTime);

	//	/*if (CurrentAction->Strategy.IsValid())
	//	{
	//		UE_LOG(LogTemp, Warning, TEXT("strat is valid"));
	//		UE_LOG(LogTemp, Warning, TEXT("%s"), CurrentAction->Strategy->Complete() ? TEXT("true") : TEXT("false"));
	//	}*/

	//	if (CurrentAction->Strategy->Complete())
	//	{
	//		UE_LOG(LogTemp, Warning, TEXT("%s complete"), *CurrentAction->Name);
	//		CurrentAction->Stop();
	//		CurrentAction->EvaluateEffects();
	//		
	//		if (TheActionPlan->AgentActions.Num() > 0)
	//		{
	//			CurrentAction = TheActionPlan->AgentActions[0];
	//			TheActionPlan->AgentActions.RemoveAt(0);
	//			CurrentAction->Start();
	//		}
	//		else
	//		{
	//			CurrentAction = nullptr;
	//		}

	//		if (TheActionPlan->AgentActions.Num() == 0)
	//		{
	//			UE_LOG(LogTemp, Warning, TEXT("Plan complete"));
	//			LastGoal = CurrentGoal;
	//			//CurrentGoal = nullptr;
	//		}
	//	}
	//}
	
	/*if (CurrentAction.IsValid())
	{
		CurrentActionText = "Action: " + ActionStackComponent->GetCurrentAction()->Name;
	}
	else
	{
		CurrentActionText = "Action: ";
	}*/
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
	CalculatePlan();

	if (TheActionPlan.IsValid() && TheActionPlan->AgentActions.Num() > 0)
	{
		CurrentGoal = TheActionPlan->AgentGoal;
		//UE_LOG(LogTemp, Warning, TEXT("Goal: %s with %d actions in plan"), *CurrentGoal->Name, TheActionPlan->AgentActions.Num());
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
	TheActionPlan = nullptr;
	/*CurrentAction = nullptr;
	CurrentGoal = nullptr;*/
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

	for (UGoapBelief* uB : uBeliefs)
	{
		if (uB)
		{
			Factory->AddBelief(uB->Name, [uB, this]() { return uB->Evaluate(AI); });
		}

		/*if (UBooleanBelief* BB = Cast<UBooleanBelief>(uB))
		{
			Factory->AddBelief(uB->Name, [uB, this]() { return uB->Evaluate(AI); });
		}
		else if (UBlackboardBoolBelief* BBB = Cast<UBlackboardBoolBelief>(uB))
		{
			Factory->AddBelief(uB->Name, [uB, this]() { return uB->Evaluate(AI); });
		}
		else if (ULocationBelief* LB = Cast<ULocationBelief>(uB))
		{
			Factory->AddLocationBelief(uB->Name, LB->DistanceThreshold, )
		}*/
	}
	
	
	
	//auto AgentMoving = MakeShared<AgentBeliefs>("AgentMoving");

	//Factory->AddBelief("Nothing", []() { return false; });

	///*Factory->AddBelief("AgentIdle", [this]()
	//	{
	//		if (!AI) return false;
	//		return AI->GetMoveStatus() == EPathFollowingStatus::Idle;
	//	});*/

	/*Factory->AddBelief("AgentMoving", [this]()
		{
			if (!AI) return false;
			return AI->GetMoveStatus() == EPathFollowingStatus::Moving;
		});

	Factory->AddBelief("AgentStaminaLow", [this]() {return NPC->Stamina < 20.0;});

	Factory->AddBelief("AgentIsRested", [this]() {return NPC->Stamina > 90.0;});

	Factory->AddLocationBelief("AgentAtRechargeStation", 5.f, RechargeStation);

	Factory->AddLocationBelief("AgentAtSupplyStation", 5.f, SupplyStation);

	Factory->AddLocationBelief("AgentAtMine", 5.f, Mine);

	Factory->AddLocationBelief("AgentAtOilWell", 5.f, OilWell);

	Factory->AddLocationBelief("AgentAtRefinery", 5.f, Refinery);

	Factory->AddLocationBelief("AgentAtSupplyShipmentStation", 5.f, SupplyShipmentStation);

	Factory->AddBelief("PlayerInChaseRange", [this]() { return AI->GetBlackboardComponent()->GetValueAsBool("bCanSeePlayer"); });

	Factory->AddBelief("PlayerInAttackRange", [this]() { return FVector::Dist(this->GetOwner()->GetActorLocation(), AI->GetBlackboardComponent()->GetValueAsVector("PlayerLocation")) < 5.f; });

	Factory->AddBelief("AttackingPlayer", []() { return false; });*/

	//Factory->AddBelief("OutOfOil")
}

void UGoapComponent::SetupAction()
{
	for (UGoapActionsSet* uA : uActions)
	{
		if (uA)
		{
			UGoapActionStrategyBase* Strategy = nullptr;

			if (uA->StrategyInstance)
			{
				Strategy = DuplicateObject<UGoapActionStrategyBase>(uA->StrategyInstance, this);
				
				if (UIdleStrategy* IdleStrategy = Cast<UIdleStrategy>(Strategy))
				{
					UIdleStrategy* RuntimeIdle = MakeStrategy<UIdleStrategy>(this, IdleStrategy->Duration);
					Actions.Add(GoapAction::Builder(uA->Name).WithStrategy(RuntimeIdle).WithCost(uA->Cost).AddPreconditions(uA->PreConditions).AddEffects(uA->Effects).Build());
				}
				else if (UMoveStrategy* MoveStrategy = Cast<UMoveStrategy>(Strategy))
				{
					UMoveStrategy* RuntimeMove = MakeStrategy<UMoveStrategy>(this, AI, MoveStrategy->TargetActor);
					Actions.Add(GoapAction::Builder(uA->Name).WithStrategy(RuntimeMove).WithCost(uA->Cost).AddPreconditions(uA->PreConditions).AddEffects(uA->Effects).Build());
				}
				else if (UPatrolStrategy* PatrolStrategy = Cast<UPatrolStrategy>(Strategy))
				{
					UPatrolStrategy* RuntimePatrol = MakeStrategy<UPatrolStrategy>(this, AI);
					Actions.Add(GoapAction::Builder(uA->Name).WithStrategy(RuntimePatrol).WithCost(uA->Cost).AddPreconditions(uA->PreConditions).AddEffects(uA->Effects).Build());
				}
				else if (URechargeStrategy* RechargeStrategy = Cast<URechargeStrategy>(Strategy))
				{
					URechargeStrategy* RuntimeRecharge = MakeStrategy<URechargeStrategy>(this, AI, RechargeStrategy->StaminaGoal);
					Actions.Add(GoapAction::Builder(uA->Name).WithStrategy(RuntimeRecharge).WithCost(uA->Cost).AddPreconditions(uA->PreConditions).AddEffects(uA->Effects).Build());
				}
				else if (UChasePlayerStrategy* ChaseStrategy = Cast<UChasePlayerStrategy>(Strategy))
				{
					UChasePlayerStrategy* RuntimeChase = MakeStrategy<UChasePlayerStrategy>(this, AI);
					Actions.Add(GoapAction::Builder(uA->Name).WithStrategy(RuntimeChase).WithCost(uA->Cost).AddPreconditions(uA->PreConditions).AddEffects(uA->Effects).Build());
				}
			}
		}
	}
	
	//UIdleStrategy* Idlethis = MakeStrategy<UIdleStrategy>(this, 1);
	//Actions.Add(GoapAction::Builder("Relax").WithStrategy(Idlethis).AddEffect("Nothing").Build());

	////Actions.Add(GoapAction::Builder("Patrol").WithStrategy(MakeShared<UPatrolStrategy>(AI, this->GetWorld())).AddEffect("AgentMoving").Build());
	//UPatrolStrategy* Patrolthis = MakeStrategy<UPatrolStrategy>(this, AI);
	//Actions.Add(GoapAction::Builder("Patrol").WithStrategy(Patrolthis).AddEffect("AgentMoving").Build());

	////Actions.Add(GoapAction::Builder("MoveToRestArea").WithStrategy(MakeShared<MoveStrategy>(AI, [this]() -> FVector { return RechargeStation; })).AddPrecondition("AgentStaminaLow").AddEffect("AgentAtRechargeStation").Build());
	//UMoveStrategy* Movethis = MakeStrategy<UMoveStrategy>(this, AI, RechargeStationActor);
	//Actions.Add(GoapAction::Builder("MoveToRestArea").WithStrategy(Movethis).AddPrecondition("AgentStaminaLow").AddEffect("AgentAtRechargeStation").Build());

	////Actions.Add(GoapAction::Builder("Recharge").WithStrategy(MakeShared<RechargeStrategy>(AI, 100.f)).AddPrecondition("AgentAtRechargeStation").AddEffect("AgentIsRested").Build());
	//URechargeStrategy* Rechargethis = MakeStrategy<URechargeStrategy>(this, AI, 100.f);
	//Actions.Add(GoapAction::Builder("Recharge").WithStrategy(Rechargethis).AddPrecondition("AgentAtRechargeStation").AddEffect("AgentIsRested").Build());

	////Actions.Add(GoapAction::Builder("ChasePlayer").WithStrategy(MakeShared<ChasePlayerStrategy>(AI)).AddPrecondition("PlayerInChaseRange").AddEffect("PlayerInAttackRange").Build());
	//UChasePlayerStrategy* Chasethis = MakeStrategy<UChasePlayerStrategy>(this, AI);
	//Actions.Add(GoapAction::Builder("ChasePlayer").WithStrategy(Chasethis).AddPrecondition("PlayerInChaseRange").AddEffect("PlayerInAttackRange").Build());

	/*Actions.Add(GoapAction::Builder("AttackPlayer").WithStrategy(MakeShared<UIdleStrategy>()).AddPrecondition("PlayerInAttackRange").AddEffect("AttackingPlayer").Build());*/
}

void UGoapComponent::SetupGoals()
{
	for (UGoapGoalObject* uG : uGoals)
	{
		if (uG)
		{
			Goals.Add(GoapGoal::Builder(uG->Name).WithPriority(uG->Priority).WithDesiredEffect(uG->DesiredEffects).Build());
		}
	}
	
	/*Goals.Add(GoapGoal::Builder("ChillOut").WithPriority(1).WithDesiredEffect("Nothing").Build());

	Goals.Add(GoapGoal::Builder("SecureTheArea").WithPriority(2).WithDesiredEffect("AgentMoving").Build());

	Goals.Add(GoapGoal::Builder("KeepStaminaUp").WithPriority(3).WithDesiredEffect("AgentIsRested").Build());

	Goals.Add(GoapGoal::Builder("SeekAndDestroy").WithPriority(4).WithDesiredEffect("PlayerInAttackRange").Build());*/
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

	TSharedPtr<ActionPlan> potentialPlan = GoapPlanner->Plan(this, goalsToCheck, LastGoal);

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
	/*LastNPCState.Stamina = Stamina;*/

	LastNPCState.bCanSeePlayer = AI_BlackBoard->GetValueAsBool("bCanSeePlayer");
}

bool UGoapComponent::HasNPCStateChanged()
{
	if (LastNPCState.bCanSeePlayer != AI_BlackBoard->GetValueAsBool("bCanSeePlayer"))
	{
		//UE_LOG(LogTemp, Warning, TEXT("%s"), AI->GetBlackboardComponent()->GetValueAsBool("bCanSeePlayer") ? TEXT("True") : TEXT("false"));
		return true;
	}

	return false;
}

