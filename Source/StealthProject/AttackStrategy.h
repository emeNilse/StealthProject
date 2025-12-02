// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "IGoapActionStrategy.h"
#include "CountdownTimer.h"

/**
 * 
 */
class STEALTHPROJECT_API AttackStrategy : public IGoapActionStrategy
{
private:
	bool bComplete;
	TUniquePtr<CountdownTimer> Timer;

public:
	AttackStrategy(float duration);
	~AttackStrategy();

	virtual void Start() override;

	virtual void Tick(float DeltaTime) override;

	virtual void Stop() override;

	virtual bool CanPerform() const override { return true; };

	virtual bool Complete() const override { return bComplete; };
};
