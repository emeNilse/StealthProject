
#include "MoveStrategy.h"
#include "NavigationSystem.h"
#include "Navigation/PathFollowingComponent.h"
#include "NavigationPath.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Pawn.h"


void UMoveStrategy::Start()
{
	if (!AI)
	{
		Status = EStrategyStatus::Failed;
		return;
	}

	AActor* RuntimeTarget = nullptr;

	if (bEnableBlackboardLocation)
	{
		if (AActor* BlackboardTarget = Cast<AActor>(AI->GetBlackboardComponent()->GetValueAsObject(BlackboardKey)))
		{
			RuntimeTarget = BlackboardTarget;
			if (!IsValid(RuntimeTarget))
			{
				Status = EStrategyStatus::Failed;
				return;
			}
		}
		else
		{
			FVector BlackboardVector = AI->GetBlackboardComponent()->GetValueAsVector(BlackboardKey);
			Destination = BlackboardVector;
		}
	}
	else
	{
		RuntimeTarget = TargetActor.Get();
		if (!IsValid(RuntimeTarget))
		{
			Status = EStrategyStatus::Failed;
			return;
		}
	}
	
	if (IsValid(RuntimeTarget))
	{
		Destination = RuntimeTarget->GetActorLocation();
	}

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

void UMoveStrategy::Stop()
{
	if (!AI) return;

	AI->StopMovement();
}

bool UMoveStrategy::CanPerform() const
{
	return true;
}

bool UMoveStrategy::Complete() const
{
	if (!AI) return false;
	
	return FVector::Dist(AI->GetPawn()->GetActorLocation(), Destination) < WithinMinimumRange;
}

//I have read somewhere (can't remember the source, soz) that Dynamic Range Cost can be one option
//to give an action a "cost". Basically, if the AI has to choose between two movement options (going to different objects)
//then it should take the "nearest" option since it requires less walking. I had to remake the GoapAction's Cost
//functions because of this and implement a lambda that you see in GoapComponent. This is Costly (pun not intended), 
//so I have a boolean, DynamicRangeCostActive, that is false by default and should return the Cost set for the 
//strategy in the editor. I have tested it, and it seems to work fine, but couldn't think of a proper use of it in
//the game nor what is a reasonable cost to return from the function. Since cost is based on distance, the cost can go up to
//large numbers, so I tried rounding the cost to a "max cost", but haven't had enough testing to know if it is a viable max. 
float UMoveStrategy::GetCost(AAI_Controller* inAI, float DefaultCost) const
{
	if (DynamicRangeCostActive)
	{
		if (!inAI || !TargetActor.IsValid()) return DefaultCost;
		float RawDistance = GetRemainingDistance(inAI, TargetActor.Get()->GetActorLocation());
		float MaxCost = 20.f;
		return FMath::Min(RawDistance / 100.0f, MaxCost);
	}
	else
	{
		return DefaultCost;
	}
}

//FVector::Dist returns a straight line distance-to-destination, this method specifically looks for the remaining
//distance to walk to the destination via the NavMesh
float UMoveStrategy::GetRemainingDistance(AAI_Controller* inAI, const FVector& targetDestination) const
{
	if(!inAI || !inAI->GetPawn()) return 0.0f;
	
	UWorld* World = inAI->GetWorld();
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
