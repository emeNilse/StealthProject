// Fill out your copyright notice in the Description page of Project Settings.


#include "GoapComponent.h"
#include "Navigation/PathFollowingComponent.h"
#include "BeliefFactory.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Math/UnrealMathUtility.h"
#include "AI_Controller.h"
#include "ActionPlan.h"

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
	AI_BlackBoard = AI->GetBlackboardComponent();

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

	StatTimer->Tick(DeltaTime);

	//Update plan and current action if there is no
	if (!CurrentAction.IsValid())
	{
		//Calculate Plan
		CalculatePlan();

		if (TheActionPlan.IsValid() && TheActionPlan->AgentActions.Num() > 0)
		{
			if (AI)
			{
				AI->StopMovement();
			}

			CurrentGoal = TheActionPlan->AgentGoal;
			UE_LOG(LogTemp, Warning, TEXT("Goal: %s with %d actions in plan"), *CurrentGoal->Name, TheActionPlan->AgentActions.Num());
			CurrentAction = TheActionPlan->AgentActions.Pop();
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
				UE_LOG(LogTemp, Warning, TEXT("Preconditions met"));
				CurrentAction = nullptr;
				CurrentGoal = nullptr;
			}
		}
	}

	//if there is a currentaction, execute
	if (TheActionPlan.IsValid() && CurrentAction.IsValid())
	{
		CurrentAction->Tick(DeltaTime);

		if (CurrentAction->bComplete)
		{
			UE_LOG(LogTemp, Warning, TEXT("%s complete"), *CurrentAction->Name);
			CurrentAction->Stop();
			CurrentAction = nullptr;

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
	BeliefFactory Factory = BeliefFactory(this, Beliefs);

	Factory.AddBelief("Nothing", []() { return false; });

	Factory.AddBelief("AgentIdle", [this]()
		{
			if (!AI) return false;
			return AI->GetMoveStatus() == EPathFollowingStatus::Idle;
		});

	Factory.AddBelief("AgentMoving", [this]()
		{
			if (!AI) return false;
			return AI->GetMoveStatus() == EPathFollowingStatus::Moving;
		});

	Factory.AddBelief("AgentStaminaLow", [this]() {return Stamina < 10.0;});

	Factory.AddLocationBelief("AgentAtRechargeStation", 5.f, RechargeStation);

	Factory.AddBelief("PlayerInChaseRange", [this]() { return AI->GetBlackboardComponent()->GetValueAsBool("bCanSeePlayer"); });

	Factory.AddBelief("PlayerInAttackRange", [this]() { return FVector::Dist(this->GetOwner()->GetActorLocation(), AI->GetBlackboardComponent()->GetValueAsVector("PlayerLocation")) < 5.f; });
}

void UGoapComponent::SetupAction()
{
	Actions.Add(GoapAction::Builder("Relax").WithStrategy(MakeShared<IdleStrategy>(5)).AddEffect(*Beliefs["Nothing"]).Build());

	Actions.Add(GoapAction::Builder("Patrol").WithStrategy(MakeShared<PatrolStrategy>(AI, this->GetWorld())).AddEffect(*Beliefs["AgentMoving"]).Build());

	Actions.Add(GoapAction::Builder("MoveToRestArea").WithStrategy(MakeShared<MoveStrategy>(AI, [this]() -> FVector { return RechargeStation; })).AddEffect(*Beliefs["AgentAtRechargeStation"]).Build());

	Actions.Add(GoapAction::Builder("Recharge").WithStrategy(MakeShared<IdleStrategy>(5)).AddPrecondition(*Beliefs["AgentAtRechargeStation"]).AddEffect(*Beliefs["AgentIsRested"]).Build());

	Actions.Add(GoapAction::Builder("ChasePlayer").WithStrategy(MakeShared<MoveStrategy>(AI, [this]() -> FVector { return AI->GetBlackboardComponent()->GetValueAsVector("PlayerLocation"); })).AddPrecondition(*Beliefs["PlayerInChaseRange"]).AddEffect(*Beliefs["PlayerInAttackRange"]).Build());

	Actions.Add(GoapAction::Builder("AttackPlayer").WithStrategy(MakeShared<AttackStrategy>(1)).AddPrecondition(*Beliefs["PlayerInAttackRange"]).AddEffect(*Beliefs["AttackingPlayer"]).Build());
}

void UGoapComponent::SetupGoals()
{
	Goals.Add(GoapGoal::Builder("ChillOut").WithPriority(1).WithDesiredEffect(*Beliefs["Nothing"]).Build());

	Goals.Add(GoapGoal::Builder("SecureTheArea").WithPriority(1).WithDesiredEffect(*Beliefs["AgentMoving"]).Build());

	Goals.Add(GoapGoal::Builder("KeepStaminaUp").WithPriority(2).WithDesiredEffect(*Beliefs["AgentIsRested"]).Build());

	Goals.Add(GoapGoal::Builder("SeekAndDestroy").WithPriority(3).WithDesiredEffect(*Beliefs["AttackingPlayer"]).Build());
}

void UGoapComponent::SetupTimers()
{
	//Update stats every 2 seconds
	StatTimer = MakeUnique<CountdownTimer>(2.f);

	StatTimer->OnTimerStop.AddLambda([this]() { UpdateStats(); StatTimer->Start(); });

	StatTimer->Start();
}

void UGoapComponent::UpdateStats()
{
	Stamina += FVector::Dist(this->GetOwner()->GetActorLocation(), RechargeStation) < 5.f ? 20 : -5;

	Stamina = FMath::Clamp(Stamina, 0, 100);
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
	CurrentAction = nullptr;
	CurrentGoal = nullptr;
	bShouldReplan = true;
}

