// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "StealthGameState.generated.h"

/**
 * 
 */
UCLASS()
class STEALTHPROJECT_API AStealthGameState : public AGameStateBase
{
	GENERATED_BODY()
public:

	AStealthGameState();

	virtual void BeginPlay() override;

	void SetGlobalAlert();
};
