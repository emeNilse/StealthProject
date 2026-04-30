
#pragma once

#include "CoreMinimal.h"
#include "GoapAction.h"

//This Node class is used by the Goap Planner when coming up with a plan

class STEALTHPROJECT_API Node
{
public:
	Node* Parent;

	TSharedPtr<GoapAction> Action;

	TSet<TSharedPtr<AgentBeliefs>> RequiredEffects;

	TArray<Node*> Leaves;

	float Cost = 0.f;

	float GCost = 0.f;
	float HCost = 0.f;
	float FCost() const { return GCost + HCost; }

	bool IsLeafDead() const
	{
		return Leaves.Num() == 0 && Action == nullptr;
	}

	Node(Node* parent, TSharedPtr<GoapAction> action, TSet<TSharedPtr<AgentBeliefs>> effects, float cost);

	~Node();
};
