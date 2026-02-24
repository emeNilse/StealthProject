
#pragma once

#include "CoreMinimal.h"

//The Action Plan contains the Goal, chosen Actions, and the total Cost.
//Ties in with Goap Planner Interface and Goap Planner.

class GoapAction;
class GoapGoal;

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
