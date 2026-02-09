// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
//#include "GoapGoal.h"
//#include "GoapAction.h"
#include "AgentBeliefs.h"
//#include "GoapComponent.h"
#include "Node.h"
#include "ActionPlan.h"
#include "AI_Controller.h"
#include "GoapPlannerInterface.h"
//#include "GoapPlanner.generated.h"


//class ActionPlan;
class GoapGoal;
class GoapAction;
/**
 * 
 */

class UGoapComponent;

class STEALTHPROJECT_API GoapPlanner : public IGoapPlannerInterface
{
	

public:
	GoapPlanner();

	~GoapPlanner();

	virtual TSharedPtr<ActionPlan> Plan(UGoapComponent* agent, AAI_Controller* inAI, TSet<TSharedPtr<GoapGoal>> goals, TSharedPtr<GoapGoal> mostRecentGoal) override;

	bool FindPath(Node* parent, AAI_Controller* inAI, TSet<TSharedPtr<GoapAction>> actions);

	bool HasMatchingEffect(TSet<TSharedPtr<AgentBeliefs>>& actionEffects, TSharedPtr<AgentBeliefs> belief);
};
