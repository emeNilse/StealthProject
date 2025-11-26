// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionPlan.h"

ActionPlan::ActionPlan()
{
}

ActionPlan::~ActionPlan()
{
}

ActionPlan::ActionPlan(GoapGoal* goal, TArray<GoapAction*> actions, float totalCost)
{
	AgentGoal = goal;
	AgentActions = actions;
	TotalCost = totalCost;
}
