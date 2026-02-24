// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GoapActionStrategyBase.h"
#include "RechargeStrategy.generated.h"

class AAI_Controller;
class ANPC;


//One of the first strategies I made, in hindsight I would make this inherit from UProcessStrategy I think
//Purpose of this strategy is to imply "healing" or similar stat modification
UCLASS(Blueprintable, EditInlineNew)
class STEALTHPROJECT_API URechargeStrategy : public UGoapActionStrategyBase
{
	GENERATED_BODY()

public:
	virtual UGoapActionStrategyBase* CreateRunTimeInstance(UObject* Outer, AAI_Controller* inAI) const override
	{
		URechargeStrategy* Runtime = NewObject<URechargeStrategy>(Outer);
		Runtime->AI = inAI;
		Runtime->StaminaGoal = StaminaGoal;
		Runtime->PowerDrainRate = PowerDrainRate;
		return Runtime;
	}
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float StaminaGoal;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float PowerDrainRate = 10.f;

	virtual void Start() override;

	virtual void Tick(float DeltaTime) override;

	virtual void Stop() override;

	virtual bool CanPerform() const override;

	virtual bool Complete() const override;

	float GetPowerDrainRate() const { return PowerDrainRate; }


private:
	AAI_Controller* AI;

	ANPC* NPC;

};
