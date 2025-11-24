// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "IGoapActionStrategy.h"
#include "CountdownTimer.h"


//class CountdownTimer;
/**
 * 
 */
class STEALTHPROJECT_API IdleStrategy : public IGoapActionStrategy
{
private:

	bool bComplete;
	TUniquePtr<CountdownTimer> Timer;

public:
	IdleStrategy(float duration);
	~IdleStrategy();

	virtual void Start() override;

	virtual void Tick(float DeltaTime) override;

	//will idle need a Stop()?

	virtual bool CanPerform() const override { return true; }

	virtual bool Complete() const override { return bComplete; }
};
