// Fill out your copyright notice in the Description page of Project Settings.


#include "ShootAtTargetStrategy.h"
#include "GoapComponent.h"
#include "SquadComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
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
	UE_LOG(LogTemp, Warning, TEXT("Action: %f"), FVector::DistSquared(AI->GetPawn()->GetActorLocation(), Target->GetActorLocation()));
	
	if (FVector::DistSquared(AI->GetPawn()->GetActorLocation(), Target->GetActorLocation()) > DistanceThreshold * DistanceThreshold)
	{
		AI->GetBlackboardComponent()->SetValueAsVector("ShootingPosition", FVector::ZeroVector);
		SelfReference->bBehindCover = false;
		SelfReference->CallUnCrouch();
		Status = EStrategyStatus::Failed;
	}
	
	if (AI->GetBlackboardComponent()->GetValueAsBool("bCanCrouch"))
	{
		SelfReference->bBehindCover = true;
		SelfReference->CallCrouch();
	}

	FRotator lookAtRot = UKismetMathLibrary::FindLookAtRotation(SelfReference->GetActorLocation(), Target->GetActorLocation());

	SelfReference->SetActorRotation(lookAtRot);

	SelfReference->RayCast();
}

bool UShootAtTargetStrategy::Complete() const
{
	return false;
}
