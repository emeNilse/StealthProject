// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GoapBelief.h"
#include "BooleanBelief.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, EditInlineNew)
class STEALTHPROJECT_API UBooleanBelief : public UGoapBelief
{
	GENERATED_BODY()
	
public:

	UPROPERTY(EditAnywhere)
	bool bDeafaultValue = false;

	virtual bool Evaluate(AAI_Controller* AI) const override
	{
		return bDeafaultValue;
	}
};
