// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ResourceStorage.generated.h"


UINTERFACE(MinimalAPI)
class UResourceStorage : public UInterface
{
	GENERATED_BODY()
};


class STEALTHPROJECT_API IResourceStorage
{
	GENERATED_BODY()


public:
	virtual int32 GetAmount(FName Resource) const = 0;
	virtual int32 GetCapacity(FName Resource) const = 0;

	virtual bool CanTake(FName Resource, int32 Amount) const = 0;
	virtual bool CanDeposit(FName Resource, int32 Amount) const = 0;

	virtual void Take(FName Resource, int32 Amount) = 0;
	virtual void Deposit(FName Resource, int32 Amount) = 0;

};
