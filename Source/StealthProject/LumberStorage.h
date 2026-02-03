// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseStation.h"
#include "ResourceStorage.h"
#include "LumberStorage.generated.h"

UCLASS()
class STEALTHPROJECT_API ALumberStorage : public ABaseStation, public IResourceStorage
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALumberStorage();

	virtual bool Interact_Implementation(AActor* interactor, EInteractionType type) override;

	virtual void GatherWorldFacts_Implementation(TArray<FWorldFact>& OutFacts) override;

	virtual int32 GetAmount(FName Resource) const override;
	virtual int32 GetCapacity(FName Resource) const override;

	virtual bool CanTake(FName Resource, int32 Amount) const override;
	virtual bool CanDeposit(FName Resource, int32 Amount) const override;

	virtual void Take(FName Resource, int32 Amount) override;
	virtual void Deposit(FName Resource, int32 Amount) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	/*void TakeLumber(int amount);

	void RefillStorage(int amount);*/

private:

	int CurrentLumberAmount;

	int MaxLumberAmount = 100;

	const FName LumberResource;
};
