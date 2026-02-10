// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GoapBelief.h"
#include "AI_Controller.h"
#include "LocationBelief.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, EditInlineNew)
class STEALTHPROJECT_API ULocationBelief : public UGoapBelief
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere)
	float DistanceThreshold = 5.f;

	UPROPERTY(EditAnywhere)
	TSoftObjectPtr<AActor> TargetActor;

	virtual bool Evaluate(AAI_Controller* AI) const override
	{
		if (!AI || !TargetActor.IsValid())
		{
			return false;
		}

		return FVector::Dist(AI->GetPawn()->GetActorLocation(), TargetActor->GetActorLocation()) <= DistanceThreshold;
	}
	
};
