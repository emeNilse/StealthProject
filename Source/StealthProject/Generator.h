// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseStation.h"
#include "Generator.generated.h"

/**
 * 
 */
UCLASS()
class STEALTHPROJECT_API AGenerator : public ABaseStation
{
	GENERATED_BODY()
	
public:

	void DrainPower(float drainrate);

	void ReFuel(float fuelrate);

	UFUNCTION(BlueprintImplementableEvent)
	void OnDrainPower();

	UFUNCTION(BlueprintCallable)
	float GetCurrentJuice() const { return CurrentJuice; }

private:

	UPROPERTY(EditAnywhere)
	float CurrentJuice = 100;
};
