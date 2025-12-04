// Fill out your copyright notice in the Description page of Project Settings.


#include "PatrolStrategy.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"

PatrolStrategy::PatrolStrategy(AAI_Controller* inAI, UWorld* inWorld)
{
	AI = inAI;
	World = inWorld;
}

PatrolStrategy::~PatrolStrategy()
{
}

void PatrolStrategy::Start()
{
	if (!World) return;
	bComplete = false;
	NPC = Cast<ANPC>(AI->GetPawn());
	FVector currentLocation = NPC->GetActorLocation();
	NPC->SetPatrolPath(FindClosestPatrolPath(World, currentLocation));
	NOofPoints = NPC->GetPatrolPath()->Num();
	BBC = AI->GetBlackboardComponent();
	Index = BBC->GetValueAsInt("PatrolPathIndex");
	FVector Point = NPC->GetPatrolPath()->GetPatrolPoint(Index);
	GlobalPoint = NPC->GetPatrolPath()->GetActorTransform().TransformPosition(Point);

	AI->MoveToLocation(GlobalPoint);
}

void PatrolStrategy::Tick(float DeltaTime)
{
	DistanceToTarget = FVector::Dist(NPC->GetActorLocation(), GlobalPoint);
	
	if (/*!bComplete &&*/ DistanceToTarget < 100.f)
	{
		//IncrementPathIndex();
		Index = ++Index % NOofPoints;

		BBC->SetValueAsInt("PatrolPathIndex", Index);

		FVector Point = NPC->GetPatrolPath()->GetPatrolPoint(Index);
		GlobalPoint = NPC->GetPatrolPath()->GetActorTransform().TransformPosition(Point);

		AI->MoveToLocation(GlobalPoint);
	}
}

void PatrolStrategy::Stop()
{
	bComplete = true;
	AI->StopMovement();
}


bool PatrolStrategy::Complete() const
{
	return ((Index + 1) % (NOofPoints + 1) == 0);
}

float PatrolStrategy::GetRemainingDistance(AAI_Controller* inAI, const FVector& targetDestination) const
{
	return 0.0f;
}

APatrolPath* PatrolStrategy::FindClosestPatrolPath(UWorld* inWorld, const FVector& inNPCLocation)
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

int PatrolStrategy::FindClostestPatrolPathPoint(APatrolPath* inPath, const FVector& inNPCLocation)
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

void PatrolStrategy::IncrementPathIndex()
{
	
}
