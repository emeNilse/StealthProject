// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GoapBelief.h"
#include "AI_Controller.h"
#include "MovementStatusBelief.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, EditInlineNew)
class STEALTHPROJECT_API UMovementStatusBelief : public UGoapBelief
{
	GENERATED_BODY()

public: 

	UPROPERTY(EditAnywhere)
	TEnumAsByte<EPathFollowingStatus::Type> RequiredStatus;

	virtual bool Evaluate(AAI_Controller* AI) const override
	{
		if (!AI)
		{
			return false;
		}

		return AI->GetMoveStatus() == RequiredStatus;
	}
	
};
