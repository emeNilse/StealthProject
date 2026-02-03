// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractionStrategy.h"
#include "ResourceStorage.h"
#include "CheckResourceStorageStrategy.generated.h"

/**
 * 
 */
UCLASS()
class STEALTHPROJECT_API UCheckResourceStorageStrategy : public UInteractionStrategy
{
	GENERATED_BODY()
	

public:
	UPROPERTY(EditAnywhere)
	FName Resource;

	virtual void Start() override;


};
