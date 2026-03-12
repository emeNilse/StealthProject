// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GoapBelief.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AI_Controller.h"
#include "BlackboardLocationBelief.generated.h"

//Beliefs based on Locations saved in the Blackboard
UCLASS(Blueprintable, EditInlineNew)
class STEALTHPROJECT_API UBlackboardLocationBelief : public UGoapBelief
{
	GENERATED_BODY()
	
public:

	UPROPERTY(EditAnywhere)
	FName BlackboardKey;

	UPROPERTY(EditAnywhere)
	float DistanceThreshold = 5.f;

	virtual bool Evaluate(AAI_Controller* AI) const override
	{
		if (!AI) return false;

		AActor* Target = Cast<AActor>(AI->GetBlackboardComponent()->GetValueAsObject(BlackboardKey));

		if (!Target) return false;

		return FVector::Dist(AI->GetPawn()->GetActorLocation(), Target->GetActorLocation()) <= DistanceThreshold;
	}
};
