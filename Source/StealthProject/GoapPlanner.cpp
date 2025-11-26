// Fill out your copyright notice in the Description page of Project Settings.


#include "GoapPlanner.h"

GoapPlanner::GoapPlanner()
{
}

GoapPlanner::~GoapPlanner()
{
}

ActionPlan* GoapPlanner::Plan(UGoapComponent* agent, TSet<GoapGoal*>& goals, GoapGoal* mostRecentGoal)
{
	TArray<GoapGoal*> orderdGoals = goals.Array();

	orderdGoals = orderdGoals.FilterByPredicate([](GoapGoal* G)
		{
			for (TSharedPtr<AgentBeliefs> belief : G->DesiredEffects)
			{
				if (!belief->Evaluate())
				{
					return true;
				}
			}
			return false;
		});

	orderdGoals.Sort([mostRecentGoal](const GoapGoal& A, const GoapGoal& B)
		{
			float APriority = (&A == mostRecentGoal) ? A.Priority - 0.01f : A.Priority;

			float BPriority = (&B == mostRecentGoal) ? B.Priority - 0.01f : B.Priority;

			return APriority > BPriority;
		});

	for (GoapGoal* goal : orderdGoals)
	{
		Node* goalNode = new Node(nullptr, nullptr, goal->DesiredEffects, 0);

		if (FindPath(goalNode, agent->Actions))
		{
			if (goalNode->IsLeafDead())
			{
				continue;
			}

			TArray<GoapAction*> goapActionStack;
			//Node* currentNode = goalNode;
			while (goalNode->Leaves.Num() > 0)
			{
				goalNode->Leaves.Sort([](const Node& A, const Node& B)
					{
						return A.Cost < B.Cost;
					});

				Node* cheapestLeaf = goalNode->Leaves[0];

				goalNode = cheapestLeaf;

				goapActionStack.Push(cheapestLeaf->Action);
			}

			//to make it a "stack"
			Algo::Reverse(goapActionStack);

			return new ActionPlan(goal, goapActionStack, goalNode->Cost);
		}
	}
	
	UE_LOG(LogTemp, Warning, TEXT("No plan found"));
	return nullptr;
}

bool GoapPlanner::FindPath(Node* parent, TSet<GoapAction*>& actions)
{
	TArray<GoapAction*> orderdActions = actions.Array();

	orderdActions.Sort([](const GoapAction& A, const GoapAction& B)
		{
			return A.Cost < B.Cost;
		});
	
	for (GoapAction* action : orderdActions)
	{
		TSet<TSharedPtr<AgentBeliefs>> requiredEffects = parent->RequiredEffects;

		for (TSharedPtr<AgentBeliefs> belief : requiredEffects)
		{
			if (belief->Evaluate())
			{
				requiredEffects.Remove(belief);
			}
		}

		if (requiredEffects.Num() == 0)
		{
			return true;
		}

		for (TSharedPtr<AgentBeliefs> belief : requiredEffects)
		{
			if (action->Effects.Contains(belief))
			{
				TSet<TSharedPtr<AgentBeliefs>> newRequiredEffects = requiredEffects;
				newRequiredEffects = newRequiredEffects.Difference(action->Effects);
				newRequiredEffects = newRequiredEffects.Union(action->Preconditions);

				TSet<GoapAction*> newAvailableActions = actions;
				newAvailableActions.Remove(action);

				Node* newNode = new Node(parent, action, newRequiredEffects, parent->Cost + action->Cost);

				if (FindPath(newNode, newAvailableActions))
				{
					parent->Leaves.Add(newNode);
					newRequiredEffects.Difference(newNode->Action->Preconditions);
				}

				if (newRequiredEffects.Num() == 0)
				{
					return true;
				}
			}
		}
	}

	return parent->Leaves.Num() > 0;
	//return false;
}

