
#pragma once

#include "CoreMinimal.h"
#include "GoapActionStrategyBase.h"
#include "LastKnownPlayerLocationStrategy.generated.h"

//AI moves to the player's last known location after losing "sight" of the player.

UCLASS(Blueprintable, EditInlineNew)
class STEALTHPROJECT_API ULastKnownPlayerLocationStrategy : public UGoapActionStrategyBase
{
	GENERATED_BODY()
	
public:

	virtual UGoapActionStrategyBase* CreateRunTimeInstance(UObject* Outer, AAI_Controller* inAI) const override
	{
		ULastKnownPlayerLocationStrategy* Runtime = NewObject<ULastKnownPlayerLocationStrategy>(Outer);
		Runtime->AI = inAI;
		return Runtime;
	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float WithinMinimumRange = 50.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxStuckTime = 3.f;

	virtual void Start() override;

	virtual void Tick(float DeltaTime) override;

	virtual void Stop() override;

	virtual bool CanPerform() const override { return !Complete(); };

	virtual bool Complete() const override;

private:

	AAI_Controller* AI;

	FVector Destination;

	float StuckTimer;
};
