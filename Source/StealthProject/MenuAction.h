// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActionBehaviour.h"
#include "MenuAction.generated.h"

/**
 * 
 */
UCLASS()
class STEALTHPROJECT_API UMenuAction : public UActionBehaviour
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<UUserWidget> WidgetClass;

protected:
	UPROPERTY()
	TObjectPtr<UUserWidget> WidgetInstance;

	bool bIsDone = false;

public:
	virtual void OnBegin_Implementation(bool bFirstTime) override;
	virtual void OnEnd_Implementation() override;
	virtual bool IsDone_Implementation() const override;
	virtual void RequestDone() override;
};
