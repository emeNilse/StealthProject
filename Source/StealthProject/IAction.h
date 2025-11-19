// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "IAction.generated.h"

UINTERFACE(Blueprintable)
class STEALTHPROJECT_API UActionInterface : public UInterface
{
	GENERATED_BODY()
};


class STEALTHPROJECT_API IActionInterface
{
	GENERATED_BODY()

public:
	/*IAction();
	virtual ~IAction();*/

	UFUNCTION(BlueprintNativeEvent)
	void OnBegin(bool bFirstTime);

	UFUNCTION(BlueprintNativeEvent)
	void OnUpdate();

	UFUNCTION(BlueprintNativeEvent)
	void OnEnd();

	UFUNCTION(BlueprintNativeEvent)
	bool IsDone() const;

	/*virtual FString ToString() const
	{
		return TEXT("Action");
	}*/
};
