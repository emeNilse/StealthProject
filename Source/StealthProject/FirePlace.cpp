
#include "FirePlace.h"


AFirePlace::AFirePlace()
{
	PrimaryActorTick.bCanEverTick = true;
}

bool AFirePlace::Interact_Implementation(AActor* interactor, EInteractionType type)
{
	//Super::Interact_Implementation(interactor, type);
	
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
	
	if (UWorldFactRegistry* Registry = GetGameInstance()->GetSubsystem<UWorldFactRegistry>())
	{
		Registry->Register(this);
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
			DeactivateFire();
		}
	}
}

void AFirePlace::IgniteFire()
{
	TimeActive = 10.f;
	bFireActive = true;
}



