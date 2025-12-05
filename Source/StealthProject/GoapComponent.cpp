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

	StatTimerInterval = 1.f;
	StatTimerRemaining = 1.f;

	//SetupTimers();
	Factory = MakeUnique<BeliefFactory>(this, Beliefs);
	SetupBeliefs();
	SetupAction();
	SetupGoals();

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
}


// Called every frame
void UGoapComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	StatTimerRemaining -= DeltaTime;
	if (StatTimerRemaining <= 0.f)
	{
		UpdateStats();
		StatTimerRemaining = StatTimerInterval;
	}

	/*StatTimer->Tick(DeltaTime);*/

	if (!CurrentAction.IsValid() || HasNPCStateChanged())
	{
		CalculatePlan();
		UpdateNPCState();

		if (TheActionPlan.IsValid() && TheActionPlan->AgentActions.Num() > 0)
		{

			CurrentGoal = TheActionPlan->AgentGoal;
			UE_LOG(LogTemp, Warning, TEXT("Goal: %s with %d actions in plan"), *CurrentGoal->Name, TheActionPlan->AgentActions.Num());
			CurrentAction = TheActionPlan->AgentActions[0];
			TheActionPlan->AgentActions.RemoveAt(0);
			UE_LOG(LogTemp, Warning, TEXT("Popped action: %s"), *CurrentAction->Name);

			//verify all precondition effects are true
			bool bAllPreconditionsMet = true;

			for (TSharedPtr<AgentBeliefs>& b : CurrentAction->Preconditions)
			{
				if (!b->Evaluate())
				{
					bAllPreconditionsMet = false;
					break;
				}
			}

			if (bAllPreconditionsMet)
			{
				CurrentAction->Start();
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("Preconditions not met"));
				CurrentAction = nullptr;
				CurrentGoal = nullptr;
			}
		}

		UE_LOG(LogTemp, Warning, TEXT("new plan"));
	}
	
	//if there is a currentaction, execute
	if (TheActionPlan.IsValid() && CurrentAction.IsValid())
	{
		CurrentAction->Tick(DeltaTime);

		/*if (CurrentAction->Strategy.IsValid())
		{
			UE_LOG(LogTemp, Warning, TEXT("strat is valid"));
			UE_LOG(LogTemp, Warning, TEXT("%s"), CurrentAction->Strategy->Complete() ? TEXT("true") : TEXT("false"));
		}*/

		if (CurrentAction->Strategy->Complete())
		{
			UE_LOG(LogTemp, Warning, TEXT("%s complete"), *CurrentAction->Name);
			CurrentAction->Stop();
			CurrentAction->EvaluateEffects();
			
			if (TheActionPlan->AgentActions.Num() > 0)
			{
				CurrentAction = TheActionPlan->AgentActions[0];
				TheActionPlan->AgentActions.RemoveAt(0);
				CurrentAction->Start();
			}
			else
			{
				CurrentAction = nullptr;
			}

			if (TheActionPlan->AgentActions.Num() == 0)
			{
				UE_LOG(LogTemp, Warning, TEXT("Plan complete"));
				LastGoal = CurrentGoal;
				CurrentGoal = nullptr;
			}
		}
	}
}

void UGoapComponent::SetupBeliefs()
{
	//BeliefFactory
	
	//auto AgentMoving = MakeShared<AgentBeliefs>("AgentMoving");

	Factory->AddBelief("Nothing", []() { return false; });

	/*Factory->AddBelief("AgentIdle", [this]()
		{
			if (!AI) return false;
			return AI->GetMoveStatus() == EPathFollowingStatus::Idle;
		});*/

	Factory->AddBelief("AgentMoving", [this]()
		{
			if (!AI) return false;
			return AI->GetMoveStatus() == EPathFollowingStatus::Moving;
		});

	Factory->AddBelief("AgentStaminaLow", [this]() {return NPC->Stamina < 20.0;});

	Factory->AddBelief("AgentIsRested", [this]() {return NPC->Stamina > 90.0;});

	Factory->AddLocationBelief("AgentAtRechargeStation", 50.f, RechargeStation);

	Factory->AddBelief("PlayerInChaseRange", [this]() { return AI->GetBlackboardComponent()->GetValueAsBool("bCanSeePlayer"); });

	Factory->AddBelief("PlayerInAttackRange", [this]() { return FVector::Dist(this->GetOwner()->GetActorLocation(), AI->GetBlackboardComponent()->GetValueAsVector("PlayerLocation")) < 5.f; });

	Factory->AddBelief("AttackingPlayer", []() { return false; });
}

void UGoapComponent::SetupAction()
{
	Actions.Add(GoapAction::Builder("Relax").WithStrategy(MakeShared<IdleStrategy>(2)).AddEffect("Nothing").Build());

	Actions.Add(GoapAction::Builder("Patrol").WithStrategy(MakeShared<PatrolStrategy>(AI, this->GetWorld())).AddEffect("AgentMoving").Build());

	Actions.Add(GoapAction::Builder("MoveToRestArea").WithStrategy(MakeShared<MoveStrategy>(AI, [this]() -> FVector { return RechargeStation; })).AddPrecondition("AgentStaminaLow").AddEffect("AgentAtRechargeStation").Build());

	Actions.Add(GoapAction::Builder("Recharge").WithStrategy(MakeShared<RechargeStrategy>(AI, 100.f)).AddPrecondition("AgentAtRechargeStation").AddEffect("AgentIsRested").Build());

	//Actions.Add(GoapAction::Builder("ChasePlayer").WithStrategy(MakeShared<MoveStrategy>(AI, [this]() -> FVector { return AI->GetBlackboardComponent()->GetValueAsVector("PlayerLocation"); })).AddPrecondition("PlayerInChaseRange").AddEffect("PlayerInAttackRange").Build());

	//Actions.Add(GoapAction::Builder("AttackPlayer").WithStrategy(MakeShared<AttackStrategy>(1)).AddPrecondition("PlayerInAttackRange").AddEffect("AttackingPlayer").Build());
}

void UGoapComponent::SetupGoals()
{
	Goals.Add(GoapGoal::Builder("ChillOut").WithPriority(1).WithDesiredEffect("Nothing").Build());

	Goals.Add(GoapGoal::Builder("SecureTheArea").WithPriority(2).WithDesiredEffect("AgentMoving").Build());

	Goals.Add(GoapGoal::Builder("KeepStaminaUp").WithPriority(3).WithDesiredEffect("AgentIsRested").Build());

	//Goals.Add(GoapGoal::Builder("SeekAndDestroy").WithPriority(4).WithDesiredEffect("AttackingPlayer").Build());
}

//void UGoapComponent::SetupTimers()
//{
//	//Update stats every 2 seconds
//	StatTimer = MakeUnique<CountdownTimer>(1.f);
//
//	StatTimer->OnTimerStop.AddLambda([this]() { UpdateStats(); StatTimer->Start(); });
//
//	StatTimer->Start();
//}

void UGoapComponent::UpdateStats()
{
	//Stamina += FVector::Dist(this->GetOwner()->GetActorLocation(), RechargeStation) < 200.f ? 5 : -5;
	//Stamina += CurrentAction == ""

	/*if (Stamina < 10)
	{
		RequestReplan();
	}*/

	
}

void UGoapComponent::CalculatePlan()
{
	int priorityLevel = CurrentGoal ? CurrentGoal->Priority : 0;

	TSet<TSharedPtr<GoapGoal>> goalsToCheck = Goals;

	if (CurrentGoal.IsValid())
	{
		//Current goal exists, checking goals with higher priority
		TSet<TSharedPtr<GoapGoal>> filteredGoals;

		for (TSharedPtr<GoapGoal> g : goalsToCheck)
		{
			if (g->Priority > priorityLevel)
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
}

void UGoapComponent::RequestReplan()
{
	bShouldReplan = true;
}

void UGoapComponent::UpdateNPCState()
{
	LastNPCState.Stamina = Stamina;

	LastNPCState.bCanSeePlayer = AI_BlackBoard->GetValueAsBool("bCanSeePlayer");
}

bool UGoapComponent::HasNPCStateChanged()
{
	/*if (FMath::Abs(LastNPCState.Stamina - Stamina) > 5)
	{
		return true;
	}*/
	
	if (LastNPCState.bCanSeePlayer != AI_BlackBoard->GetValueAsBool("bCanSeePlayer"))
	{
		return true;
	}

	return false;
}

