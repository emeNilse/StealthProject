// Fill out your copyright notice in the Description page of Project Settings.


#include "GetFlankPositionStrategy.h"
#include "GoapComponent.h"
#include "SquadComponent.h"

void UGetFlankPositionStrategy::Start()
{
	if (!AI)
	{
		Status = EStrategyStatus::Failed;
		return;
	}

	USquadComponent* squadComponent = AI->GetGoap()->GetSquadComponent();
	if (!squadComponent)
	{
		Status = EStrategyStatus::Failed;
		return;
	}

	squadComponent->OnComplete.BindUObject(this, &UGetFlankPositionStrategy::AcquiredFlankingposition);
	squadComponent->RequestFlankingPosition();
	Status = EStrategyStatus::Running;
}

void UGetFlankPositionStrategy::Tick(float DeltaTime)
{
	if (Complete())
	{
		Status = EStrategyStatus::Succeeded;
	}
}

bool UGetFlankPositionStrategy::Complete() const
{
	return bCalculationComplete;
}

void UGetFlankPositionStrategy::AcquiredFlankingposition()
{
	bCalculationComplete = true;
}
