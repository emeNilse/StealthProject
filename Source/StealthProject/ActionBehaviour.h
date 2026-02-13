// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "IAction.h"
#include "ActionBehaviour.generated.h"


UCLASS(Blueprintable, EditInlineNew)
class STEALTHPROJECT_API UActionBehaviour : public UObject, public IActionInterface
{
	GENERATED_BODY()

public:
	
	UFUNCTION(BlueprintCallable)
	void Initialize(APlayerController* InPlayerController);

	virtual void OnBegin_Implementation(bool bFirstTime) override {}

	virtual void OnUpdate_Implementation() override {}

	virtual void OnEnd_Implementation() override {}

	virtual bool IsDone_Implementation() const override { return true; }

	UFUNCTION(BlueprintCallable)
	virtual void RequestDone();

protected:
	UPROPERTY()
	APlayerController* OwningPlayerController;
};
