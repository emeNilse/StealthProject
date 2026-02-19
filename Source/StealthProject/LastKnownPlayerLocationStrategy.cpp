// Fill out your copyright notice in the Description page of Project Settings.


#include "LastKnownPlayerLocationStrategy.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "NavigationSystem.h"
#include "Navigation/PathFollowingComponent.h"
#include "NavigationPath.h"

void ULastKnownPlayerLocationStrategy::Start()
{
	if (!AI)
	{
		Status = EStrategyStatus::Failed;
		return;
	}

	Destination = AI->GetBlackboardComponent()->GetValueAsVector("PlayerLocation");
	AI->MoveToLocation(Destination);
	Status = EStrategyStatus::Running;
}

void ULastKnownPlayerLocationStrategy::Tick(float DeltaTime)
{
	if (Complete())
	{
		Status = EStrategyStatus::Succeeded;
	}
	else if (!CanPerform())
	{
		Status = EStrategyStatus::Failed;
	}
	else
	{
		if (AI->GetMoveStatus() == EPathFollowingStatus::Waiting || AI->GetMoveStatus() == EPathFollowingStatus::Idle)
		{
			StuckTimer += DeltaTime;
			if (StuckTimer >= MaxStuckTime)
			{
				Status = EStrategyStatus::Failed;
			}
		}
		else
		{
			StuckTimer = 0.0f;
		}
	}
}

void ULastKnownPlayerLocationStrategy::Stop()
{
	if (!AI) return;

	AI->StopMovement();
}

bool ULastKnownPlayerLocationStrategy::Complete() const
{
	if (!AI) return false;

	return FVector::Dist(AI->GetPawn()->GetActorLocation(), Destination) < WithinMinimumRange;
}
