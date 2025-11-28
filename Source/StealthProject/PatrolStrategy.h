// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "IGoapActionStrategy.h"
#include "AI_Controller.h"
#include "PatrolPath.h"
/**
 * 
 */
class STEALTHPROJECT_API PatrolStrategy : public IGoapActionStrategy
{
public:
	PatrolStrategy();
	~PatrolStrategy();

	TFunction<FVector()> Destination;

	AAI_Controller* AI;

	virtual void Start() override;

	virtual void Tick(float DeltaTime) override;

	virtual void Stop() override;

	virtual bool CanPerform() const override;

	virtual bool Complete() const override;

	float GetRemainingDistance(AAI_Controller* inAI, const FVector& targetDestination) const;

	APatrolPath* FindClosestPatrolPath(UWorld* inWorld, const FVector& inNPCLocation);

};
