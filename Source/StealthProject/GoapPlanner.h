
#pragma once

#include "CoreMinimal.h"
#include "AgentBeliefs.h"
#include "Node.h"
#include "ActionPlan.h"
#include "AI_Controller.h"
#include "GoapPlannerInterface.h"

//The GoapPlanner is responsible for coming up with a plan for the AI.
//The planners sorts Goals on priority and Actions on cost, and iterates over goals until it has found a plan that is viable.
//I want to expand on the planner so that it makes use of another search algorithm (I was wondering if something like A* is possible).
//Beacuse the current set up will be less efficient the more goals and actions I give the AI.
//Inherits from Goap Planner Interface to set up Plan.

class GoapGoal;
class GoapAction;
class UGoapComponent;
DECLARE_LOG_CATEGORY_EXTERN(LogGOAP, Log, All);
DECLARE_LOG_CATEGORY_EXTERN(LogGOAPGOAL, Log, All);

class STEALTHPROJECT_API GoapPlanner : public IGoapPlannerInterface
{
public:
	GoapPlanner();

	~GoapPlanner();

	virtual TSharedPtr<ActionPlan> Plan(UGoapComponent* agent, AAI_Controller* inAI, TSet<TSharedPtr<GoapGoal>> goals, TSharedPtr<GoapGoal> mostRecentGoal) override;

	bool FindPath(Node* parent, AAI_Controller* inAI, TSet<TSharedPtr<GoapAction>> actions, int32 depth);

	Node* FindPathAStar(Node* parentNode, AAI_Controller* inAI, TSet<TSharedPtr<GoapAction>> actions);

	bool HasMatchingEffect(TSet<TSharedPtr<AgentBeliefs>>& actionEffects, TSharedPtr<AgentBeliefs> belief);

	float Heuristic(const TSet<TSharedPtr<AgentBeliefs>>& effects);

	bool IsInClosedAndBetterCost(Node* node, const TArray<Node*>& ClosedList);

	bool AreEffectsEqual(TSet<TSharedPtr<AgentBeliefs>>& A, TSet<TSharedPtr<AgentBeliefs>>& B);

	TArray<TSharedPtr<GoapAction>> BuildPlan(Node* endNode);

private:
	//TArray<TUniquePtr<Node>> AllNodes;

	//I was using this for "Pretty printing", but thinking I'll scrap it
	/*FString Indent(int32 Depth)
	{
		return FString::ChrN(Depth * 2, TEXT(' '));
	}*/
};
