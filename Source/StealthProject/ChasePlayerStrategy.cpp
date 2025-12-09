// Fill out your copyright notice in the Description page of Project Settings.


#include "ChasePlayerStrategy.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "NavigationSystem.h"
#include "Navigation/PathFollowingComponent.h"
#include "NavigationPath.h"

ChasePlayerStrategy::ChasePlayerStrategy(AAI_Controller* inAI) : AI(inAI)
{
}

ChasePlayerStrategy::~ChasePlayerStrategy()
{
}

void ChasePlayerStrategy::Start()
{
	if (!AI) return;

	Player = Cast<AStealthProjectCharacter>(AI->GetBlackboardComponent()->GetValueAsObject("PlayerActor"));

	if (!Player) return;

	AI->MoveToActor(Player);
}

void ChasePlayerStrategy::Tick(float DeltaTime)
{
	//Target = Player->GetActorLocation();
	AI->MoveToActor(Player);
}

void ChasePlayerStrategy::Stop()
{
	if (!AI) return;

	AI->StopMovement();
}

bool ChasePlayerStrategy::CanPerform() const
{
	return !Complete();
}

bool ChasePlayerStrategy::Complete() const
{
	if (!AI) return false;
	
	float remainingDistance = GetRemainingDistance(AI, Player);
	
	return remainingDistance < 50.f;
}

float ChasePlayerStrategy::GetRemainingDistance(AAI_Controller* inAI, AStealthProjectCharacter* inPlayer) const
{
	if (!inAI || !inAI->GetPawn()) return 0.0f;

	UWorld* World = AI->GetWorld();
	if (!World) return 0.f;

	UNavigationSystemV1* NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(World);
	if (!NavSys) return 0.0f;

	UNavigationPath* NavPath = NavSys->FindPathToLocationSynchronously(World, inAI->GetPawn()->GetActorLocation(), Player->GetActorLocation(), inAI->GetPawn());

	if (!NavPath || NavPath->PathPoints.Num() < 2) return 0.f;

	float Length = 0.f;

	for (int i = 1; i < NavPath->PathPoints.Num(); i++)
	{
		Length += FVector::Dist(NavPath->PathPoints[i - 1], NavPath->PathPoints[i]);
	}

	return Length;
}
