// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "AI_Controller.h"
#include "GoapBelief.generated.h"

/**
 * 
 */

UENUM(BlueprintType)
enum class EFloatComparison : uint8
{
	Less,
	LessOrEqual,
	Greater,
	GreaterOrEqual,
	Equal,
	Range
};

UCLASS(Abstract, Blueprintable, EditInlineNew)
class STEALTHPROJECT_API UGoapBelief : public UObject
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FString Name;

	virtual bool Evaluate(class AAI_Controller* AI) const PURE_VIRTUAL(UGoapBelief::Evaluate, return false;);
};
