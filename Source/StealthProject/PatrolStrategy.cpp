// Fill out your copyright notice in the Description page of Project Settings.


#include "PatrolStrategy.h"
#include "Kismet/GameplayStatics.h"
#include "NavigationSystem.h"
#include "Navigation/PathFollowingComponent.h"
#include "NavigationPath.h"
#include "BehaviorTree/BlackboardComponent.h"

void UPatrolStrategy::Start()
{
	World = AI->GetWorld();
	if (!World)
	{
		Status = EStrategyStatus::Failed;
		return;
	}
	bPatrolling = true;

	//NPC = Cast<ANPC>(AI->GetPawn());
	//FVector currentLocation = NPC->GetActorLocation();
	//NPC->SetPatrolPath(FindClosestPatrolPath(World, currentLocation));

	UObject* Object = AI->GetBlackboardComponent()->GetValueAsObject("CurrentPatrolPath");
	MyPatrolPath = Cast<APatrolPath>(Object);

	NOofPoints = MyPatrolPath->Num();

	BBC = AI->GetBlackboardComponent();
	Index = BBC->GetValueAsInt("PatrolPathIndex");
	FVector Point = MyPatrolPath->GetPatrolPoint(Index);

	GlobalPoint = MyPatrolPath->GetActorTransform().TransformPosition(Point);
	IndexCounter = 0;

	AI->MoveToLocation(GlobalPoint);

	Status = EStrategyStatus::Running;
}

void UPatrolStrategy::Tick(float DeltaTime)
{
	if (Complete())
	{
		Status = EStrategyStatus::Succeeded;
	}

	DistanceToTarget = FVector::Dist(NPC->GetActorLocation(), GlobalPoint);
	
	if (bPatrolling && DistanceToTarget < 100.f)
	{
		Index = ++Index % NOofPoints;
		IndexCounter++;
		
		BBC->SetValueAsInt("PatrolPathIndex", Index);

		FVector Point = MyPatrolPath->GetPatrolPoint(Index);
		GlobalPoint = MyPatrolPath->GetActorTransform().TransformPosition(Point);
	
		AI->MoveToLocation(GlobalPoint);
	}
}

void UPatrolStrategy::Stop()
{
	bPatrolling = false;
	Index = NOofPoints;
	AI->StopMovement();
}

bool UPatrolStrategy::Complete() const
{
	return (IndexCounter >= NOofPoints);
}

APatrolPath* UPatrolStrategy::FindClosestPatrolPath(UWorld* inWorld, const FVector& inNPCLocation)
{
	TArray<AActor*> FoundPaths;
	UGameplayStatics::GetAllActorsOfClass(inWorld, APatrolPath::StaticClass(), FoundPaths);

	APatrolPath* Closest = nullptr;
	float BestDistSq = TNumericLimits<float>::Max();

	for (AActor* a : FoundPaths)
	{
		if (APatrolPath* Path = Cast<APatrolPath>(a))
		{
			const float DistSq = FVector::DistSquared(inNPCLocation, Path->GetActorLocation());

			if (DistSq < BestDistSq)
			{
				BestDistSq = DistSq;
				Closest = Path;
			}
		}
	}
	
	return Closest;
}

int UPatrolStrategy::FindClostestPatrolPathPoint(APatrolPath* inPath, const FVector& inNPCLocation)
{
	/*TArray<FVector> FoundPoints;
	float BestDistSq = TNumericLimits<float>::Max();

	if (auto* const NPC = Cast<ANPC>(AI->GetPawn()))
	{
		if (auto* const BBC = AI->GetBlackboardComponent())
		{
			int NOofPoints = NPC->GetPatrolPath()->Num();
			int Index = BBC->GetValueAsInt("PatrolPathIndex");

			for (int i = 0; i < NOofPoints; i++)
			{

			}
		}
		
	}*/

	return 0;
}

float UPatrolStrategy::GetRemainingDistance(AAI_Controller* inAI, const FVector& targetDestination) const
{
	if (!inAI || !inAI->GetPawn()) return 0.0f;

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
