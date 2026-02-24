
#pragma once

#include "CoreMinimal.h"
#include "GoapActionStrategyBase.h"
#include "CountdownTimer.h"
#include "IdleStrategy.generated.h"

//Goap strategy for just staying idle for a set amount of time.

UCLASS(Blueprintable, EditInlineNew)
class STEALTHPROJECT_API UIdleStrategy : public UGoapActionStrategyBase
{
	GENERATED_BODY()

public:

	virtual UGoapActionStrategyBase* CreateRunTimeInstance(UObject* Outer, AAI_Controller* inAI) const override
	{
		UIdleStrategy* Runtime = NewObject<UIdleStrategy>(Outer);
		Runtime->Duration = Duration;
		return Runtime;
	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Duration = 1.0f;

	virtual void Start() override;

	virtual void Tick(float DeltaTime) override;

	virtual void Stop() override;

	virtual bool CanPerform() const override { return true; }

	virtual bool Complete() const override { return Timer->IsFinished(); }

private:

	bool bTimerComplete;

	TUniquePtr<CountdownTimer> Timer;
};
