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

		bool dist = FVector::Dist(AI->GetPawn()->GetActorLocation(), TargetActor->GetActorLocation()) <= DistanceThreshold;
		float val = FVector::Dist(AI->GetPawn()->GetActorLocation(), TargetActor->GetActorLocation());
		FVector tar = TargetActor->GetActorLocation();
		FVector ai = AI->GetPawn()->GetActorLocation();
		UE_LOG(LogTemp, Warning, TEXT("dist = %s at %0.2f"), dist ? TEXT("true") : TEXT("false"), val);
		UE_LOG(LogTemp, Warning, TEXT("Target Actor = %s"), *tar.ToString());
		UE_LOG(LogTemp, Warning, TEXT("AI = %s"), *ai.ToString());

		return FVector::Dist(AI->GetPawn()->GetActorLocation(), TargetActor->GetActorLocation()) <= DistanceThreshold;
	}
	
};
