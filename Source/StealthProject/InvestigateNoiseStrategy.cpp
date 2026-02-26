

#include "InvestigateNoiseStrategy.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "NavigationSystem.h"
#include "Navigation/PathFollowingComponent.h"
#include "NavigationPath.h"
#include "NPC.h"

void UInvestigateNoiseStrategy::Start()
{
	if (!AI)
	{
		CallFailed();
		return;
	}

	World = AI->GetWorld();
	if (!World)
	{
		CallFailed();
		return;
	}
		
	NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(World);
	if (!NavSys)
	{
		CallFailed();
		return;
	}

	Destination = AI->GetBlackboardComponent()->GetValueAsVector("NoiseLocation");
	NavPath = NavSys->FindPathToLocationSynchronously(World, AI->GetPawn()->GetActorLocation(), Destination, AI->GetPawn());
	if (!NavPath || NavPath->PathPoints.Num() < 2)
	{
		CallFailed();
		return;
	}

	AI->MoveToLocation(Destination);
	Status = EStrategyStatus::Running;
}

void UInvestigateNoiseStrategy::Tick(float DeltaTime)
{
	if (Complete())
	{
		APawn* Pawn = AI->GetPawn();
		ANPC* NPC = Cast<ANPC>(Pawn);
		NPC->BeginInvestigative();
		AI->GetBlackboardComponent()->SetValueAsBool("bHasInvestigated", true);
		AI->GetBlackboardComponent()->SetValueAsBool("bHeardSomething", false);
		
		Status = EStrategyStatus::Succeeded;
	}
	else if (!CanPerform())
	{
		Status = EStrategyStatus::Failed;
	}
	else
	{
		Destination = AI->GetBlackboardComponent()->GetValueAsVector("NoiseLocation");
		NavPath = NavSys->FindPathToLocationSynchronously(World, AI->GetPawn()->GetActorLocation(), Destination, AI->GetPawn());
		if (!NavPath || NavPath->PathPoints.Num() < 2)
		{
			CallFailed();
			return;
		}
		AI->MoveToLocation(Destination);

		if (AI->GetMoveStatus() == EPathFollowingStatus::Waiting || AI->GetMoveStatus() == EPathFollowingStatus::Idle)
		{
			StuckTimer += DeltaTime;
			if (StuckTimer >= MaxStuckTime)
			{
				CallFailed();
			}
		}
		else
		{
			StuckTimer = 0.0f;
		}
	}
}

void UInvestigateNoiseStrategy::Stop()
{
	if (!AI) return;

	AI->StopMovement();
}

bool UInvestigateNoiseStrategy::Complete() const
{
	if (!AI) return false;
	
	return FVector::Dist(AI->GetPawn()->GetActorLocation(), Destination) < WithinMinimumRange;
}

//Upon failure, the Blackboard booleans have to be reset otherwise the AI gets stuck in Investigation
void UInvestigateNoiseStrategy::CallFailed()
{
	APawn* Pawn = AI->GetPawn();
	ANPC* NPC = Cast<ANPC>(Pawn);
	NPC->ReturnToCalm();
	AI->GetBlackboardComponent()->SetValueAsBool("bHasInvestigated", true);
	AI->GetBlackboardComponent()->SetValueAsBool("bHeardSomething", false);
	Status = EStrategyStatus::Failed;
}
