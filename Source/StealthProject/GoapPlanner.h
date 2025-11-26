// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GoapGoal.h"
#include "GoapAction.h"
#include "AgentBeliefs.h"
#include "GoapComponent.h"
#include "Node.h"
#include "ActionPlan.h"
#include "GoapPlannerInterface.h"
//#include "GoapPlanner.generated.h"



/**
 * 
 */



class STEALTHPROJECT_API GoapPlanner : public IGoapPlannerInterface
{
	

public:
	GoapPlanner();

	~GoapPlanner();

	virtual ActionPlan* Plan(UGoapComponent* agent, TSet<GoapGoal*>& goals, GoapGoal* mostRecentGoal = nullptr) override;

	bool FindPath(Node* parent, TSet<GoapAction*>& actions);
};
