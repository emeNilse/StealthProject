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
	/*ActionBehaviour();
	~ActionBehaviour();*/

	//UFUNCTION(BlueprintCallable)
	virtual void OnBegin_Implementation(bool bFirstTime) override {}

	//UFUNCTION(BlueprintCallable)
	virtual void OnUpdate_Implementation() override {}

	//UFUNCTION(BlueprintCallable)
	virtual void OnEnd_Implementation() override {}

	//UFUNCTION(BlueprintCallable)
	virtual bool IsDone_Implementation() const override { return true; }

};
