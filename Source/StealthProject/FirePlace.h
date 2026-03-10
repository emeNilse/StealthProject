
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseStation.h"
#include "WorldFactRegistry.h"
#include "FirePlace.generated.h"

//Not in use. This is meant for Goap Presentation.
//I thought about using it in the game, but there was too much else to fix.

UCLASS()
class STEALTHPROJECT_API AFirePlace : public ABaseStation
{
	GENERATED_BODY()
	
public:	
	AFirePlace();

	virtual bool Interact_Implementation(AActor* interactor, EInteractionType type) override;

	virtual void GatherWorldFacts_Implementation(TArray<FWorldFact>& OutFacts) override;

	UFUNCTION(BlueprintCallable)
	bool IsFireActive() { return bFireActive; }

	UFUNCTION(BlueprintImplementableEvent)
	void DeactivateFire();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	void IgniteFire();

private:

	bool bFireActive;

	float TimeActive;

};
