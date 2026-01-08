// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActionBehaviour.h"
#include "OptionsMenuAction.generated.h"

/**
 * 
 */
UCLASS()
class STEALTHPROJECT_API UOptionsMenuAction : public UActionBehaviour
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<UUserWidget> WidgetClass;

protected:
	UPROPERTY()
	TObjectPtr<UUserWidget> WidgetInstance;

public:
	virtual void OnBegin_Implementation(bool bFirstTime) override;
	virtual void OnEnd_Implementation() override;
	virtual bool IsDone_Implementation() const override;

	
private:
	bool bIsDone;

};
