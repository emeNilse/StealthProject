// Fill out your copyright notice in the Description page of Project Settings.


#include "ChasePlayerStrategy.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "NavigationSystem.h"
#include "Navigation/PathFollowingComponent.h"
#include "NavigationPath.h"


//void UChasePlayerStrategy::Initialize(AAI_Controller* inAI)
//{
//	AI = inAI;
//}


void UChasePlayerStrategy::Start()
{
	if (!AI)
	{
		UE_LOG(LogTemp, Warning, TEXT("no ai"));
		Status = EStrategyStatus::Failed;
		return;
	}
		

	Player = Cast<AStealthProjectCharacter>(AI->GetBlackboardComponent()->GetValueAsObject("PlayerActor"));
	UE_LOG(LogTemp, Warning, TEXT("chase player strat"));

	if (!Player)
	{
		UE_LOG(LogTemp, Warning, TEXT("can't find player"));
		Status = EStrategyStatus::Failed;
		return;
	}

	AI->MoveToActor(Player);
	Status = EStrategyStatus::Running;
}

void UChasePlayerStrategy::Tick(float DeltaTime)
{
	//Target = Player->GetActorLocation();
	AI->MoveToActor(Player);

	if (Complete())
	{
		Status = EStrategyStatus::Succeeded;
	}
}

void UChasePlayerStrategy::Stop()
{
	if (!AI) return;

	AI->StopMovement();
}

bool UChasePlayerStrategy::CanPerform() const
{
	return !Complete();
}

bool UChasePlayerStrategy::Complete() const
{
	if (!AI) return false;
	
	float remainingDistance = GetRemainingDistance(AI, Player);
	
	return remainingDistance < 50.f;
}

float UChasePlayerStrategy::GetRemainingDistance(AAI_Controller* inAI, AStealthProjectCharacter* inPlayer) const
{
	if (!inAI || !inAI->GetPawn() || !inPlayer) return 0.0f;

	UWorld* World = AI->GetWorld();
	if (!World) return 0.f;

	UNavigationSystemV1* NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(World);
	if (!NavSys) return 0.0f;

	UNavigationPath* NavPath = NavSys->FindPathToLocationSynchronously(World, inAI->GetPawn()->GetActorLocation(), inPlayer->GetActorLocation(), inAI->GetPawn());

	if (!NavPath || NavPath->PathPoints.Num() < 2) return 0.f;

	float Length = 0.f;

	for (int i = 1; i < NavPath->PathPoints.Num(); i++)
	{
		Length += FVector::Dist(NavPath->PathPoints[i - 1], NavPath->PathPoints[i]);
	}

	return Length;
}