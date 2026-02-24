
#pragma once

#include "CoreMinimal.h"
#include "GoapActionStrategyBase.h"
#include "InvestigateNoiseStrategy.generated.h"

//If the AI "heard" something, it should investigate the last location of the sound source

UCLASS(Blueprintable, EditInlineNew)
class STEALTHPROJECT_API UInvestigateNoiseStrategy : public UGoapActionStrategyBase
{
	GENERATED_BODY()
	
public:

	virtual UGoapActionStrategyBase* CreateRunTimeInstance(UObject* Outer, AAI_Controller* inAI) const override
	{
		UInvestigateNoiseStrategy* Runtime = NewObject<UInvestigateNoiseStrategy>(Outer);
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

	void CallFailed();

private:
	AAI_Controller* AI;

	FVector Destination;

	float StuckTimer;
};
