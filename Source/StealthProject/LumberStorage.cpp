// Fill out your copyright notice in the Description page of Project Settings.


#include "LumberStorage.h"

ALumberStorage::ALumberStorage()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StationTag = "Storage";

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
		Take(LumberResource, 10);
		return true;
	case EInteractionType::Storage_Refill:
		Deposit(LumberResource, 10);
		return true;
	}

	return false;
}

void ALumberStorage::GatherWorldFacts_Implementation(TArray<FWorldFact>& OutFacts)
{
	Super::GatherWorldFacts_Implementation(OutFacts);
	
	FWorldFact LumberFact;
	LumberFact.Key = "LumberStorage";
	LumberFact.Type = EWorldFactType::Int;
	LumberFact.IntValue = CurrentLumberAmount;
	LumberFact.Location = GetActorLocation();
	LumberFact.Source = TWeakObjectPtr<AActor>(this);

	OutFacts.Add(LumberFact);
}

int32 ALumberStorage::GetAmount(FName Resource) const
{
	return Resource == LumberResource ? CurrentLumberAmount : 0;
}

int32 ALumberStorage::GetCapacity(FName Resource) const
{
	return Resource == LumberResource ? MaxLumberAmount : 0;
}

bool ALumberStorage::CanTake(FName Resource, int32 Amount) const
{
	return Resource == LumberResource && CurrentLumberAmount > Amount;
}

bool ALumberStorage::CanDeposit(FName Resource, int32 Amount) const
{
	return Resource == LumberResource && CurrentLumberAmount + Amount <= MaxLumberAmount;
}

void ALumberStorage::Take(FName Resource, int32 Amount)
{
	if (!CanTake(Resource, Amount)) return;

	CurrentLumberAmount -= Amount;
}

void ALumberStorage::Deposit(FName Resource, int32 Amount)
{
	if (!CanDeposit(Resource, Amount)) return;

	CurrentLumberAmount += Amount;
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

//void ALumberStorage::TakeLumber(int amount)
//{
//	CurrentLumberAmount -= amount;
//
//	CurrentLumberAmount = FMath::Clamp(CurrentLumberAmount, 0, MaxLumberAmount);
//}
//
//void ALumberStorage::RefillStorage(int amount)
//{
//	CurrentLumberAmount += amount;
//
//	CurrentLumberAmount = FMath::Clamp(CurrentLumberAmount, 0, MaxLumberAmount);
//}

