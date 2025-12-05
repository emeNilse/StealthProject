// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "IGoapActionStrategy.h"

class AAI_Controller;
class ANPC;
/**
 * 
 */
class STEALTHPROJECT_API RechargeStrategy : public IGoapActionStrategy
{

private:
	AAI_Controller* AI;
	ANPC* NPC;
	float StaminaGoal;

public:
	RechargeStrategy(AAI_Controller* inAI, float inGoal);
	~RechargeStrategy();

	virtual void Start() override;

	virtual void Tick(float DeltaTime) override;

	virtual void Stop() override;

	virtual bool CanPerform() const override;

	virtual bool Complete() const override;

};
