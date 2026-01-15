// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GoapBelief.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AI_Controller.h"
#include "BlackboardBoolBelief.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, EditInlineNew)
class STEALTHPROJECT_API UBlackboardBoolBelief : public UGoapBelief
{
	GENERATED_BODY()
	
public:

	UPROPERTY(EditAnywhere)
	FName BlackboardKey;

	virtual bool Evaluate(AAI_Controller* AI) const override
	{
		if (!AI) return false;

		return AI->GetBlackboardComponent()->GetValueAsBool(BlackboardKey);
	}
};
