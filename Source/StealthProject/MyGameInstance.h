// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "ActionStack.h"
#include "MyGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class STEALTHPROJECT_API UMyGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, Category = "Subsystem")
	UActionStack* GetActionStack() const;

};
