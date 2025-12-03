// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
//#include "GoapGoal.h"
//#include "GoapAction.h"
#include "AgentBeliefs.h"
//#include "GoapComponent.h"
#include "Node.h"
#include "ActionPlan.h"
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

	virtual TSharedPtr<ActionPlan> Plan(UGoapComponent* agent, TSet<TSharedPtr<GoapGoal>> goals, TSharedPtr<GoapGoal> mostRecentGoal = nullptr) override;

	bool FindPath(Node* parent, TSet<TSharedPtr<GoapAction>> actions);

	bool HasMatchingEffect(TSet<TSharedPtr<AgentBeliefs>>& actionEffects, TSharedPtr<AgentBeliefs> belief);
};
