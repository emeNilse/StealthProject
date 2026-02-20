// Fill out your copyright notice in the Description page of Project Settings.


#include "InvestigateNoiseStrategy.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "NavigationSystem.h"
#include "Navigation/PathFollowingComponent.h"
#include "NavigationPath.h"
#include "NPC.h"

void UInvestigateNoiseStrategy::Start()
{
	if (!AI)
	{
		Status = EStrategyStatus::Failed;
		return;
	}

	Destination = AI->GetBlackboardComponent()->GetValueAsVector("NoiseLocation");
	AI->MoveToLocation(Destination);
	Status = EStrategyStatus::Running;
}

void UInvestigateNoiseStrategy::Tick(float DeltaTime)
{
	if (Complete())
	{
		APawn* pawn = AI->GetPawn();
		ANPC* npc = Cast<ANPC>(pawn);
		npc->ReturnToCalm();
		AI->GetBlackboardComponent()->SetValueAsBool("bHasInvestigated", true);
		AI->GetBlackboardComponent()->SetValueAsBool("bHeardSomething", false);
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

void UInvestigateNoiseStrategy::Stop()
{
	if (!AI) return;

	AI->StopMovement();
}

bool UInvestigateNoiseStrategy::Complete() const
{
	if (!AI) return false;
	
	return FVector::Dist(AI->GetPawn()->GetActorLocation(), Destination) < WithinMinimumRange;
}
