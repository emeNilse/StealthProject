// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GoapBelief.h"
#include "AI_Controller.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "DistanceToActorBelief.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, EditInlineNew)
class STEALTHPROJECT_API UDistanceToActorBelief : public UGoapBelief
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere)
	float MaxDistance = 5.f;

	UPROPERTY(EditAnywhere)
	FName BlackboardKey;

	virtual bool Evaluate(AAI_Controller* AI) const override
	{
		if (!AI)
		{
			return false;
		}

		AActor* Target = Cast<AActor>(AI->GetBlackboardComponent()->GetValueAsObject(BlackboardKey));

		if (!Target)
		{
			return false;
		}

		return FVector::Dist(AI->GetPawn()->GetActorLocation(), Target->GetActorLocation()) <= MaxDistance;
	}
	
};
