// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "IGoapActionStrategy.h"
#include "AI_Controller.h"

/**
 * 
 */
class STEALTHPROJECT_API MoveStrategy : public IGoapActionStrategy
{
public:
	MoveStrategy(AAI_Controller* inAI, TFunction<FVector()> inDestination);
	~MoveStrategy();

	TFunction<FVector()> Destination;

	AAI_Controller* AI;

	virtual void Start() override;

	virtual void Tick(float DeltaTime) override;

	virtual void Stop() override;

	virtual bool CanPerform() const override;

	virtual bool Complete() const override;

	float GetRemainingDistance(AAI_Controller* inAI, const FVector& targetDestination) const;
};
