// Fill out your copyright notice in the Description page of Project Settings.


#include "MoveStrategy.h"
#include "NavigationSystem.h"
#include "Navigation/PathFollowingComponent.h"
#include "NavigationPath.h"
#include "GameFramework/Pawn.h"

//void UMoveStrategy::Initialize(AAI_Controller* inAI, TSoftObjectPtr<AActor> inActor)
//{
//	AI = inAI;
//	Destination = inActor->GetActorLocation();
//}

void UMoveStrategy::Start()
{
	if (!AI)
	{
		Status = EStrategyStatus::Failed;
		return;
	}

	AActor* RuntimeTarget = TargetActor.Get();
	if (!RuntimeTarget)
	{
		Status = EStrategyStatus::Failed;
		return;
	}

	Destination = RuntimeTarget->GetActorLocation();
	AI->MoveToLocation(Destination);
	Status = EStrategyStatus::Running;
}

void UMoveStrategy::Tick(float DeltaTime)
{
	if (Complete())
	{
		Status = EStrategyStatus::Succeeded;
	}
	else if (!CanPerform())
	{
		Status = EStrategyStatus::Failed;
	}
}

void UMoveStrategy::Stop()
{
	if (!AI) return;

	AI->StopMovement();
}

bool UMoveStrategy::CanPerform() const
{
	return true;
}

bool UMoveStrategy::HasFailed() const
{
	int timer = 0;
	if (AI->GetMoveStatus() == EPathFollowingStatus::Moving)
	{
		timer = 0;
	}
	else if (AI->GetMoveStatus() == EPathFollowingStatus::Waiting)
	{
		timer += 1;
	}
	
	return false;
}

bool UMoveStrategy::Complete() const
{
	if (!AI) return false;
	
	return FVector::Dist(AI->GetPawn()->GetActorLocation(), Destination) < 50.f;


	/*float RemainingDistance = GetRemainingDistance(AI, Destination);

	bool bPathPending = AI->GetMoveStatus() == EPathFollowingStatus::Waiting;

	return RemainingDistance < 50.f && !bPathPending;*/
}

float UMoveStrategy::GetRemainingDistance(AAI_Controller* inAI, const FVector& targetDestination) const
{
	if(!inAI || !inAI->GetPawn()) return 0.0f;
	
	UWorld* World = AI->GetWorld();
	if (!World) return 0.f;

	UNavigationSystemV1* NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(World);
	if(!NavSys) return 0.0f;

	UNavigationPath* NavPath = NavSys->FindPathToLocationSynchronously(World, inAI->GetPawn()->GetActorLocation(), targetDestination, inAI->GetPawn());

	if (!NavPath || NavPath->PathPoints.Num() < 2) return 0.f;

	float Length = 0.f;
	
	for (int i = 1; i < NavPath->PathPoints.Num(); i++)
	{
		Length += FVector::Dist(NavPath->PathPoints[i - 1], NavPath->PathPoints[i]);
	}
	
	return Length;
}
