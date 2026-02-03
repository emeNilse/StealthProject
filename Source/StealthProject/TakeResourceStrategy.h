// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractionStrategy.h"
#include "ResourceStorage.h"
#include "TakeResourceStrategy.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, EditInlineNew)
class STEALTHPROJECT_API UTakeResourceStrategy : public UInteractionStrategy
{
	GENERATED_BODY()
	
public:

	UPROPERTY(EditAnywhere)
	FName Resource;

	UPROPERTY(EditAnywhere)
	int32 Amount;

	IResourceStorage* Storage;

	virtual void Start() override;

	virtual void Stop() override;

	virtual bool CanPerform() const override;

	virtual bool Complete() const override;
};
