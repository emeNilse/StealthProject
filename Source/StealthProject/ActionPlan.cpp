// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionPlan.h"
#include "GoapAction.h"
#include "GoapGoal.h"

ActionPlan::ActionPlan()
{
}

ActionPlan::~ActionPlan()
{
}

ActionPlan::ActionPlan(TSharedPtr<GoapGoal> goal, TArray<TSharedPtr<GoapAction>> actions, float totalCost)
{
	AgentGoal = goal;
	AgentActions = actions;
	TotalCost = totalCost;
}
