// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GoapBelief.h"
#include "GoapBeliefSet.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class STEALTHPROJECT_API UGoapBeliefSet : public UObject
{
	GENERATED_BODY()
	
public:

	UPROPERTY(EditAnywhere, Instanced)
	TArray<UGoapBelief*> Beliefs;

};
