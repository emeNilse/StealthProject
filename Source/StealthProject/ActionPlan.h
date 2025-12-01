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
	TSharedPtr<GoapGoal> AgentGoal;

	TArray<TSharedPtr<GoapAction>> AgentActions;

	float TotalCost;

	ActionPlan(TSharedPtr<GoapGoal> goal, TArray<TSharedPtr<GoapAction>> actions, float totalCost);

	ActionPlan();

	~ActionPlan();

};
