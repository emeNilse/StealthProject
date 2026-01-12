// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "ActionStack.h"
#include "MyGameInstance.generated.h"

class UActionBehaviour;
/**
 * 
 */
UCLASS()
class STEALTHPROJECT_API UMyGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	UMyGameInstance();

	virtual void Init() override;

	UFUNCTION(BlueprintCallable, Category = "Subsystem")
	UActionStack* GetActionStack() const;

protected:
	UPROPERTY(EditDefaultsOnly, Category="Startup")
	TSubclassOf<UActionBehaviour> StartupAction;

	UPROPERTY()
	UActionStack* Stack;

private:
	void HandlePostLoadMap(UWorld* loadedLevel);

	bool bStartupActionPushed = false;

	void TryPushStartupAction(UWorld* World, ELevelTick TickType, float DeltaSeconds);

};
