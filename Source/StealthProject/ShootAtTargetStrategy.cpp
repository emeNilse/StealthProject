// Fill out your copyright notice in the Description page of Project Settings.


#include "ShootAtTargetStrategy.h"
#include "GoapComponent.h"
#include "SquadComponent.h"
#include "Kismet/KismetMathLibrary.h"

void UShootAtTargetStrategy::Start()
{
	if (!AI)
	{
		Status = EStrategyStatus::Failed;
		return;
	}

	Target = AI->GetGoap()->GetSquadComponent()->GetSquad()->GetCurrentTarget();
	if (!Target)
	{
		Status = EStrategyStatus::Failed;
		return;
	}

	SelfReference = AI->GetMyNPC();
	if (!SelfReference)
	{
		Status = EStrategyStatus::Failed;
		return;
	}

	Status = EStrategyStatus::Running;
}

void UShootAtTargetStrategy::Tick(float DeltaTime)
{
	FRotator lookAtRot = UKismetMathLibrary::FindLookAtRotation(SelfReference->GetActorLocation(), Target->GetActorLocation());

	SelfReference->SetActorRotation(lookAtRot);

	SelfReference->RayCast();
}

bool UShootAtTargetStrategy::Complete() const
{
	return false;
}
