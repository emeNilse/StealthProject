// Fill out your copyright notice in the Description page of Project Settings.


#include "RandomLocationStrategy.h"

void URandomLocationStrategy::Start()
{
	if (!AI)
	{
		Status = EStrategyStatus::Failed;
		return;
	}

	const int MaxAttempts = 5;
	int Attempts = 0;
	bool bLocationfound = false;

	while (Attempts < MaxAttempts)
	{
		Attempts++;
		
		if (auto* const NavSys = UNavigationSystemV1::GetCurrent(GetWorld()))
		{
			FNavLocation Location;
			if (NavSys->GetRandomReachablePointInRadius(AI->GetPawn()->GetActorLocation(), SearchRadius, Location))
			{
				Destination = Location.Location;
				AI->MoveToLocation(Destination);
				Status = EStrategyStatus::Running;
				bLocationfound = true;
				break;
			}
		}
		else
		{
			Status = EStrategyStatus::Failed;
		}
	}

	if (!bLocationfound)
	{
		Status = EStrategyStatus::Failed;
	}
}

void URandomLocationStrategy::Tick(float DeltaTime)
{
	if (Complete())
	{
		Status = EStrategyStatus::Succeeded;
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

void URandomLocationStrategy::Stop()
{
	if (!AI) return;

	AI->StopMovement();
}

bool URandomLocationStrategy::Complete() const
{
	if (!AI) return false;

	return FVector::Dist(AI->GetPawn()->GetActorLocation(), Destination) < WithinMinimumRange;
}

float URandomLocationStrategy::GetCost(AAI_Controller* inAI, float DefaultCost) const
{
	if (!inAI) return DefaultCost;
	float RawDistance = GetRemainingDistance(inAI, Destination);
	float MaxCost = 20.f;
	return FMath::Min(RawDistance / 100.0f, MaxCost);
}

float URandomLocationStrategy::GetRemainingDistance(AAI_Controller* inAI, const FVector& targetDestination) const
{
	if (!inAI || !inAI->GetPawn()) return 0.0f;

	UWorld* World = inAI->GetWorld();
	if (!World) return 0.f;

	UNavigationSystemV1* NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(World);
	if (!NavSys) return 0.0f;

	UNavigationPath* NavPath = NavSys->FindPathToLocationSynchronously(World, inAI->GetPawn()->GetActorLocation(), targetDestination, inAI->GetPawn());

	if (!NavPath || NavPath->PathPoints.Num() < 2) return 0.f;

	float Length = 0.f;

	for (int i = 1; i < NavPath->PathPoints.Num(); i++)
	{
		Length += FVector::Dist(NavPath->PathPoints[i - 1], NavPath->PathPoints[i]);
	}

	return Length;
}
