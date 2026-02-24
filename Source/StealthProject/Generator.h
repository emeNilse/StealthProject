
#pragma once

#include "CoreMinimal.h"
#include "BaseStation.h"
#include "Generator.generated.h"

//Not finished yet, the idea was to let the player interact with the generator and sabotage the Recharge Stations.
//That or to have the AI feed the generator fuel to power the Recharge Station.

UCLASS()
class STEALTHPROJECT_API AGenerator : public ABaseStation
{
	GENERATED_BODY()
	
public:

	void DrainPower(float drainrate);

	void ReFuel(float fuelrate);

	UFUNCTION(BlueprintImplementableEvent)
	void OnDrainPower();

	UFUNCTION(BlueprintCallable)
	float GetCurrentJuice() const { return CurrentJuice; }

private:

	UPROPERTY(EditAnywhere)
	float CurrentJuice = 100;
};
