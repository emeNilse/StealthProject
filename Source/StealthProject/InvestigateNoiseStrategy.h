// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GoapActionStrategyBase.h"
#include "InvestigateNoiseStrategy.generated.h"

/**
 * 
 */
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

	AAI_Controller* AI;

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

	FVector Destination;

	float StuckTimer;
};
