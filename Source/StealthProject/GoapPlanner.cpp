// Fill out your copyright notice in the Description page of Project Settings.


#include "GoapPlanner.h"
#include "GoapComponent.h"
//#include "ActionPlan.h"
#include "GoapGoal.h"
#include "GoapAction.h"

GoapPlanner::GoapPlanner()
{
}

GoapPlanner::~GoapPlanner()
{
}

TSharedPtr<ActionPlan> GoapPlanner::Plan(UGoapComponent* agent, TSet<TSharedPtr<GoapGoal>> goals, TSharedPtr<GoapGoal> mostRecentGoal)
{
	TArray<TSharedPtr<GoapGoal>> orderdGoals = goals.Array();

	orderdGoals = orderdGoals.FilterByPredicate([](TSharedPtr<GoapGoal> G)
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

	orderdGoals.Sort([mostRecentGoal](const TSharedPtr<GoapGoal>& A, const TSharedPtr<GoapGoal>& B)
		{
			float APriority = (A.Get() == mostRecentGoal.Get()) ? A->Priority - 0.01f : A->Priority;

			float BPriority = (B.Get() == mostRecentGoal.Get()) ? B->Priority - 0.01f : B->Priority;

			return APriority > BPriority;
		});

	for (TSharedPtr<GoapGoal> goal : orderdGoals)
	{
		Node* goalNode = new Node(nullptr, nullptr, goal->DesiredEffects, 0);

		if (FindPath(goalNode, agent->Actions))
		{
			if (goalNode->IsLeafDead())
			{
				continue;
			}

			TArray<TSharedPtr<GoapAction>> goapActionStack;
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

			return MakeShared<ActionPlan>(goal, goapActionStack, goalNode->Cost);
		}
	}
	
	UE_LOG(LogTemp, Warning, TEXT("No plan found"));
	return nullptr;
}

bool GoapPlanner::FindPath(Node* parent, TSet<TSharedPtr<GoapAction>> actions)
{
	TArray<TSharedPtr<GoapAction>> orderdActions = actions.Array();

	orderdActions.Sort([](const TSharedPtr<GoapAction>& A, const TSharedPtr<GoapAction>& B)
		{
			return A->Cost < B->Cost;
		});
	
	for (TSharedPtr<GoapAction> action : orderdActions)
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

				TSet<TSharedPtr<GoapAction>> newAvailableActions = actions;
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

