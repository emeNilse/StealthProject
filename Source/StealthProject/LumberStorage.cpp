// Fill out your copyright notice in the Description page of Project Settings.


#include "LumberStorage.h"

// Sets default values
ALumberStorage::ALumberStorage()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

void ALumberStorage::GatherWorldFacts_Implementation(TArray<FWorldFact>& OutFacts)
{
	FWorldFact LumberFact;
	LumberFact.Key = "LumberStorage";
	LumberFact.Type = EWorldFactType::Int;
	
	LumberFact.Location = GetActorLocation();
	LumberFact.Source = TWeakObjectPtr<AActor>(this);

	OutFacts.Add(LumberFact);
}

// Called when the game starts or when spawned
void ALumberStorage::BeginPlay()
{
	Super::BeginPlay();
	
	CurrentLumberAmount = MaxLumberAmount;
}

// Called every frame
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

