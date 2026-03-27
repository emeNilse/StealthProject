// Fill out your copyright notice in the Description page of Project Settings.


#include "AimAtTargetStrategy.h"
#include "GoapComponent.h"
#include "SquadComponent.h"
#include "Kismet/KismetMathLibrary.h"

void UAimAtTargetStrategy::Start()
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

	Target = squadComponent->GetSquadTarget();
	if (!Target)
	{
		Status = EStrategyStatus::Failed;
		return;
	}

	FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(AI->GetPawn()->GetActorLocation(), Target->GetActorLocation());

	AI->GetPawn()->SetActorRotation(LookAtRotation);
	Status = EStrategyStatus::Running;
}

void UAimAtTargetStrategy::Tick(float DeltaTime)
{
	if (Complete())
	{
		Status = EStrategyStatus::Succeeded;
	}
}

bool UAimAtTargetStrategy::Complete() const
{
	FVector ToTarget = (Target->GetActorLocation() - AI->GetPawn()->GetActorLocation()).GetSafeNormal();
	return FVector::DotProduct(AI->GetPawn()->GetActorForwardVector(), ToTarget) > AimThreshold;
}
