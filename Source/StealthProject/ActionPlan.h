// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GoapAction.h"
#include "GoapGoal.h"

/**
 * 
 */
class STEALTHPROJECT_API ActionPlan
{
public:
	GoapGoal* AgentGoal;

	TArray<GoapAction*> AgentActions;

	float TotalCost;

	ActionPlan(GoapGoal* goal, TArray<GoapAction*> actions, float totalCost);

	ActionPlan();

	~ActionPlan();

};
