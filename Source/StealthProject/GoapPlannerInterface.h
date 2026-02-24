
#pragma once

#include "CoreMinimal.h"

//The Goap Planner interface
//I know what you're thinking, was this really necessary?
//This is from when I first started and the Goap tutorial I was following was for Unity...
//And in the chaos of figuring out TSharedPtrs, I forgot why I needed this, but the point(er) is that it works.

class ActionPlan;
class UGoapComponent;
class GoapGoal;
class AAI_Controller;


class IGoapPlannerInterface
{
public:
	virtual TSharedPtr<ActionPlan> Plan(UGoapComponent* agent, AAI_Controller* inAI, TSet<TSharedPtr<GoapGoal>> goals, TSharedPtr<GoapGoal> mostRecentGoal = nullptr) = 0;
};
