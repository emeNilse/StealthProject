// Fill out your copyright notice in the Description page of Project Settings.


#include "PatrolStrategy.h"
#include "Kismet/GameplayStatics.h"

PatrolStrategy::PatrolStrategy()
{
}

PatrolStrategy::~PatrolStrategy()
{
}

void PatrolStrategy::Start()
{
}

void PatrolStrategy::Tick(float DeltaTime)
{
}

void PatrolStrategy::Stop()
{
}

bool PatrolStrategy::CanPerform() const
{
	return false;
}

bool PatrolStrategy::Complete() const
{
	return false;
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
