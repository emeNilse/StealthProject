// Fill out your copyright notice in the Description page of Project Settings.


#include "LumberStorage.h"

ALumberStorage::ALumberStorage()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

bool ALumberStorage::Interact_Implementation(AActor* interactor, EInteractionType type)
{
	switch (type)
	{
	case EInteractionType::Default:
		return true;
	case EInteractionType::Storage_Check:
		return true;
	case EInteractionType::Storage_Take:
		TakeLumber(10);
		return true;
	case EInteractionType::Storage_Refill:
		RefillStorage(100);
		return true;
	}

	return false;
}

void ALumberStorage::GatherWorldFacts_Implementation(TArray<FWorldFact>& OutFacts)
{
	FWorldFact LumberFact;
	LumberFact.Key = "LumberStorage";
	LumberFact.Type = EWorldFactType::Int;
	LumberFact.IntValue = CurrentLumberAmount;
	LumberFact.Location = GetActorLocation();
	LumberFact.Source = TWeakObjectPtr<AActor>(this);

	OutFacts.Add(LumberFact);
}

void ALumberStorage::BeginPlay()
{
	Super::BeginPlay();
	
	CurrentLumberAmount = MaxLumberAmount;
}


void ALumberStorage::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ALumberStorage::TakeLumber(int amount)
{
	CurrentLumberAmount -= amount;

	CurrentLumberAmount = FMath::Clamp(CurrentLumberAmount, 0, MaxLumberAmount);
}

void ALumberStorage::RefillStorage(int amount)
{
	CurrentLumberAmount += amount;

	CurrentLumberAmount = FMath::Clamp(CurrentLumberAmount, 0, MaxLumberAmount);
}

