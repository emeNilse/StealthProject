// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseStation.h"
#include "NPC.h"
#include "Generator.h"
#include "GoapActionStrategyBase.h"
#include "RechargeStation.generated.h"

/**
 * 
 */
UCLASS()
class STEALTHPROJECT_API ARechargeStation : public ABaseStation
{
	GENERATED_BODY()
	
public:

	void TickDrainPower(ANPC* npc);

	UFUNCTION(BlueprintImplementableEvent)
	void OnStartDraining();

protected:
	UPROPERTY(EditAnywhere)
	AGenerator* Generator;
};
