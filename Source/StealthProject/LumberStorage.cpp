
#include "LumberStorage.h"

ALumberStorage::ALumberStorage()
{
	PrimaryActorTick.bCanEverTick = true;

	StationTag = "Storage";
}

bool ALumberStorage::Interact_Implementation(AActor* interactor, EInteractionType type)
{
	Super::Interact_Implementation(interactor, type);
	
	switch (type)
	{
	case EInteractionType::Default:
		return true;
	case EInteractionType::Storage_Check:
		return true;
	case EInteractionType::Storage_Take:
		Take(LumberResource, 20);
		return true;
	case EInteractionType::Storage_Refill:
		Deposit(LumberResource, 20);
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

	CurrentLumberAmount = FMath::Clamp(CurrentLumberAmount, 0, MaxLumberAmount);
}

void ALumberStorage::Deposit(FName Resource, int32 Amount)
{
	if (!CanDeposit(Resource, Amount)) return;

	CurrentLumberAmount += Amount;

	CurrentLumberAmount = FMath::Clamp(CurrentLumberAmount, 0, MaxLumberAmount);
}

void ALumberStorage::BeginPlay()
{
	Super::BeginPlay();
	
	if (UWorldFactRegistry* Registry = GetGameInstance()->GetSubsystem<UWorldFactRegistry>())
	{
		Registry->Register(this);
	}

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

