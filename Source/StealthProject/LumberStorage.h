
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseStation.h"
#include "ResourceStorage.h"
#include "WorldFactRegistry.h"
#include "LumberStorage.generated.h"

//The Lumber Storage is not in use in the Game map, this station is meant for the Goap Presentation map when it's ready.
//One of several stations that should make use of the World Fact Provider. 
//I haven't decided yet how I should implement this. Info should be shared with AI, of course, but just trying to figure out the best design.

UCLASS()
class STEALTHPROJECT_API ALumberStorage : public ABaseStation, public IResourceStorage
{
	GENERATED_BODY()
	
public:	

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
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

private:

	int CurrentLumberAmount;

	int MaxLumberAmount = 100;

	const FName LumberResource;
};
