// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "IGoapActionStrategy.generated.h"

/**
 * 
 */
UINTERFACE(Blueprintable)
class STEALTHPROJECT_API UGoapActionStrategy : public UInterface
{
	GENERATED_BODY()
	
};

class STEALTHPROJECT_API IGoapActionStrategy
{
	GENERATED_BODY()


public:
	virtual void Start() = 0;

	virtual void Tick(float DeltaTime) {}

	virtual void Stop() {}

	virtual bool CanPerform() const = 0;

	virtual bool Complete() const = 0;

};
