// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "IGoapActionStrategy.h"
#include "AI_Controller.h"
#include "NPC.h"
#include "PatrolPath.h"
/**
 * 
 */
class STEALTHPROJECT_API PatrolStrategy : public IGoapActionStrategy
{
public:
	PatrolStrategy(AAI_Controller* inAI, UWorld* inWorld);
	~PatrolStrategy();

	TFunction<FVector()> Destination;

	AAI_Controller* AI;

	UWorld* World;

	ANPC* NPC;

	int Index;

	bool bPatrolling;

	virtual void Start() override;

	virtual void Tick(float DeltaTime) override;

	virtual void Stop() override;

	virtual bool CanPerform() const override { return true; };

	virtual bool Complete() const override { return !bPatrolling; };

	float GetRemainingDistance(AAI_Controller* inAI, const FVector& targetDestination) const;

	APatrolPath* FindClosestPatrolPath(UWorld* inWorld, const FVector& inNPCLocation);

	int FindClostestPatrolPathPoint(APatrolPath* inPath, const FVector& inNPCLocation);

	FVector FindPathPoint();

	void IncrementPathIndex();

};
