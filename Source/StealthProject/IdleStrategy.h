// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GoapActionStrategyBase.h"
#include "CountdownTimer.h"
#include "IdleStrategy.generated.h"



UCLASS(Blueprintable, EditInlineNew)
class STEALTHPROJECT_API UIdleStrategy : public UGoapActionStrategyBase
{
	GENERATED_BODY()

private:
	
	bool bTimerComplete;
	TUniquePtr<CountdownTimer> Timer;

public:

	virtual UGoapActionStrategyBase* CreateRunTimeInstance(UObject* Outer, AAI_Controller* inAI) const override
	{
		UIdleStrategy* Runtime = NewObject<UIdleStrategy>(Outer);
		Runtime->Duration = Duration;
		return Runtime;
	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Duration = 1.0f;

	//virtual void Initialize(float inDuration);
	virtual void Start() override;

	virtual void Tick(float DeltaTime) override;

	virtual void Stop() override;

	virtual bool CanPerform() const override { return true; }

	virtual bool Complete() const override { return Timer->IsFinished(); }
};
