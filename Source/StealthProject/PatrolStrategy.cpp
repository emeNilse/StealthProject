
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

	NPC = Cast<ANPC>(AI->GetPawn());
	FVector currentLocation = NPC->GetActorLocation();
	NPC->SetPatrolPath(FindClosestPatrolPath(World, currentLocation));

	if (NPC->GetPatrolPath())
	{
		bPathFound = true;
	}
	else
	{
		bPathFound = false;
	}


	if (bPathFound)
	{
		NOofPoints = NPC->GetPatrolPath()->Num();

		BBC = AI->GetBlackboardComponent();
		Index = BBC->GetValueAsInt("PatrolPathIndex");
		FVector Point = NPC->GetPatrolPath()->GetPatrolPoint(Index);

		GlobalPoint = NPC->GetPatrolPath()->GetActorTransform().TransformPosition(Point);
		IndexCounter = 0;

		AI->MoveToLocation(GlobalPoint);
	}
	else
	{
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
					GlobalPoint = Location.Location;
					AI->MoveToLocation(GlobalPoint);
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
	
	Status = EStrategyStatus::Running;
}

void UPatrolStrategy::Tick(float DeltaTime)
{
	if (Complete())
	{
		if (bPathFound)
		{
			NPC->GetPatrolPath()->NoLongerUsePath();
		}
		Status = EStrategyStatus::Succeeded;
	}

	if (bPathFound)
	{
		DistanceToTarget = FVector::Dist(NPC->GetActorLocation(), GlobalPoint);

		if (bPatrolling && DistanceToTarget < 100.f)
		{
			Index = ++Index % NOofPoints;
			IndexCounter++;

			BBC->SetValueAsInt("PatrolPathIndex", Index);

			FVector Point = NPC->GetPatrolPath()->GetPatrolPoint(Index);
			GlobalPoint = NPC->GetPatrolPath()->GetActorTransform().TransformPosition(Point);

			AI->MoveToLocation(GlobalPoint);
		}
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

void UPatrolStrategy::Stop()
{
	bPatrolling = false;
	Index = NOofPoints;
	AI->StopMovement();
}

bool UPatrolStrategy::Complete() const
{
	if (bPathFound)
	{
		return (IndexCounter >= NOofPoints);
	}
	else
	{
		return FVector::Dist(AI->GetPawn()->GetActorLocation(), GlobalPoint) < WithinMinimumRange;
	}
}

APatrolPath* UPatrolStrategy::FindClosestPatrolPath(UWorld* inWorld, const FVector& inNPCLocation)
{
	TArray<AActor*> FoundPaths;
	UGameplayStatics::GetAllActorsOfClass(inWorld, APatrolPath::StaticClass(), FoundPaths);

	APatrolPath* ClosestPath = nullptr;
	float BestDistSq = TNumericLimits<float>::Max();

	for (AActor* a : FoundPaths)
	{
		if (APatrolPath* Path = Cast<APatrolPath>(a))
		{
			if (Path->IsPathTaken())
			{
				continue;
			}

			const float DistSq = FVector::DistSquared(inNPCLocation, Path->GetActorLocation());

			if (DistSq < BestDistSq)
			{
				BestDistSq = DistSq;
				ClosestPath = Path;
			}
		}
	}
	
	if (ClosestPath)
	{
		ClosestPath->PathTaken();
	}

	return ClosestPath;
}

//This function looks for the nearest patrol point belonging to a patrol path. For now though, just finding the nearest patrol path is good enough.
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

//See Move Strategy for explanation.
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
