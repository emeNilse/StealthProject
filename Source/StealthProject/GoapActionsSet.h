// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GoapActionStrategyBase.h"
#include "GoapActionsSet.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, EditInlineNew)
class STEALTHPROJECT_API UGoapActionsSet : public UObject
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Cost;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FString> PreConditions;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FString> Effects;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector Location;

	UPROPERTY(EditAnywhere, Instanced, BlueprintReadWrite)
	UGoapActionStrategyBase* StrategyInstance;
};
