// Fill out your copyright notice in the Description page of Project Settings.


#include "FirePlace.h"

// Sets default values
AFirePlace::AFirePlace()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

bool AFirePlace::Interact_Implementation(AActor* interactor, EInteractionType type)
{
	if (bFireActive) return false;

	IgniteFire();
	return true;
}

void AFirePlace::GatherWorldFacts_Implementation(TArray<FWorldFact>& OutFacts)
{
	FWorldFact FireFact;
	FireFact.Key = "FirePlace";
	FireFact.Type = EWorldFactType::Bool;
	FireFact.BoolValue = bFireActive;
	FireFact.Location = GetActorLocation();
	FireFact.Source = TWeakObjectPtr<AActor>(this);

	OutFacts.Add(FireFact);
}

void AFirePlace::BeginPlay()
{
	Super::BeginPlay();
	
	if (UWorldStateSubsystem* wss = GetGameInstance()->GetSubsystem<UWorldStateSubsystem>())
	{
		wss->RegisterProvider(this);
	}
}

void AFirePlace::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bFireActive)
	{
		TimeActive -= DeltaTime;

		if (TimeActive <= 0.f)
		{
			bFireActive = false;
		}
	}
}

void AFirePlace::IgniteFire()
{
	TimeActive = 100.f;
	bFireActive = true;
}



