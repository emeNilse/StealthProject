// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseStation.h"
#include "WorldFactProvider.h"
#include "WorldStateSubsystem.h"
#include "LumberStorage.generated.h"

UCLASS()
class STEALTHPROJECT_API ALumberStorage : public ABaseStation, public IWorldFactProvider
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALumberStorage();

	virtual void GatherWorldFacts_Implementation(TArray<FWorldFact>& OutFacts) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void TakeLumber(int amount);

	void RefillStorage(int amount);

private:

	int CurrentLumberAmount;

	int MaxLumberAmount = 100;
};
