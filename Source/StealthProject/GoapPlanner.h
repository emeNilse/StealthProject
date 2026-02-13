// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AgentBeliefs.h"
#include "Node.h"
#include "ActionPlan.h"
#include "AI_Controller.h"
#include "GoapPlannerInterface.h"

class GoapGoal;
class GoapAction;
class UGoapComponent;
DECLARE_LOG_CATEGORY_EXTERN(LogGOAP, Log, All);

class STEALTHPROJECT_API GoapPlanner : public IGoapPlannerInterface
{
	

public:
	GoapPlanner();

	~GoapPlanner();

	virtual TSharedPtr<ActionPlan> Plan(UGoapComponent* agent, AAI_Controller* inAI, TSet<TSharedPtr<GoapGoal>> goals, TSharedPtr<GoapGoal> mostRecentGoal) override;

	bool FindPath(Node* parent, AAI_Controller* inAI, TSet<TSharedPtr<GoapAction>> actions, int32 depth);

	bool HasMatchingEffect(TSet<TSharedPtr<AgentBeliefs>>& actionEffects, TSharedPtr<AgentBeliefs> belief);

	FString Indent(int32 Depth)
	{
		return FString::ChrN(Depth * 2, TEXT(' '));
	}
};
