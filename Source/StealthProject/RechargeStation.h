
#pragma once

#include "CoreMinimal.h"
#include "BaseStation.h"
#include "NPC.h"
#include "Generator.h"
#include "RechargeStrategy.h"
#include "GoapActionStrategyBase.h"
#include "RechargeStation.generated.h"

//A station where the AI's recharge their stamina.

UCLASS()
class STEALTHPROJECT_API ARechargeStation : public ABaseStation
{
	GENERATED_BODY()
	
public:

	void TickDrainPower(ANPC* NPC);

	UFUNCTION(BlueprintImplementableEvent)
	void OnStartDraining();

protected:
	UPROPERTY(EditAnywhere)
	AGenerator* Generator;
};
