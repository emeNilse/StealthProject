// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "IGoapActionStrategy.h"
#include "AI_Controller.h"
#include "StealthProjectCharacter.h"
/**
 * 
 */
class STEALTHPROJECT_API ChasePlayerStrategy : public IGoapActionStrategy
{
public:

	AAI_Controller* AI;

	AStealthProjectCharacter* Player;

	ChasePlayerStrategy(AAI_Controller* inAI);
	~ChasePlayerStrategy();

	virtual void Start() override;

	virtual void Tick(float DeltaTime) override;

	virtual void Stop() override;

	virtual bool CanPerform() const override;

	virtual bool Complete() const override;

	float GetRemainingDistance(AAI_Controller* inAI, AStealthProjectCharacter* inPlayer) const;

};
