// Fill out your copyright notice in the Description page of Project Settings.


#include "MoveStrategy.h"
#include "NavigationSystem.h"
#include "Navigation/PathFollowingComponent.h"
#include "NavigationPath.h"
#include "GameFramework/Pawn.h"

MoveStrategy::MoveStrategy(AAI_Controller* inAI, TFunction<FVector()> inDestination) : AI(inAI), Destination(inDestination)
{
	/*AI = inAI;
	Destination = inDestination;*/
}

MoveStrategy::~MoveStrategy()
{
}

void MoveStrategy::Start()
{
	if (!AI) return;

	FVector Dest = Destination();
	AI->MoveToLocation(Dest);
}

void MoveStrategy::Tick(float DeltaTime)
{

}

void MoveStrategy::Stop()
{
	if (!AI) return;

	AI->StopMovement();
}

bool MoveStrategy::CanPerform() const
{
	return !Complete();
}

bool MoveStrategy::Complete() const
{
	if (!AI) return false;

	FVector Dest = Destination();
	float RemainingDistance = GetRemainingDistance(AI, Dest);

	bool bPathPending = AI->GetMoveStatus() == EPathFollowingStatus::Waiting;

	return RemainingDistance < 50.f && !bPathPending;
}

float MoveStrategy::GetRemainingDistance(AAI_Controller* inAI, const FVector& targetDestination) const
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
