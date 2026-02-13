// Fill out your copyright notice in the Description page of Project Settings.


#include "GoapPlanner.h"
#include "GoapComponent.h"
//#include "ActionPlan.h"
#include "GoapGoal.h"
#include "GoapAction.h"

DEFINE_LOG_CATEGORY(LogGOAP);
GoapPlanner::GoapPlanner()
{
}

GoapPlanner::~GoapPlanner()
{
}

TSharedPtr<ActionPlan> GoapPlanner::Plan(UGoapComponent* agent, AAI_Controller* inAI, TSet<TSharedPtr<GoapGoal>> goals, TSharedPtr<GoapGoal> mostRecentGoal)
{
	TArray<TSharedPtr<GoapGoal>> orderdGoals = goals.Array();

	orderdGoals = orderdGoals.FilterByPredicate([inAI](TSharedPtr<GoapGoal> G)
		{
			for (TSharedPtr<AgentBeliefs> belief : G->DesiredEffects)
			{
				if (!belief->Evaluate(inAI))
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

		if (FindPath(goalNode, inAI, agent->Actions, 0))
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
	
	//UE_LOG(LogTemp, Warning, TEXT("No plan found"));
	return nullptr;
}

bool GoapPlanner::FindPath(Node* parent, AAI_Controller* inAI, TSet<TSharedPtr<GoapAction>> actions, int32 depth)
{
	//Debugging
	//UE_LOG(LogGOAP, Warning, TEXT("%sEntering FindPath | Cost: %f | RequiredEffects: %d"), *Indent(depth), parent->Cost, parent->RequiredEffects.Num());
	
	TArray<TSharedPtr<GoapAction>> orderdActions = actions.Array();

	orderdActions.Sort([](const TSharedPtr<GoapAction>& A, const TSharedPtr<GoapAction>& B)
		{
			return A->Cost < B->Cost;
		});
	
	for (TSharedPtr<GoapAction> action : orderdActions)
	{
		//Debugging
		//UE_LOG(LogGOAP, Warning, TEXT("%sEvaluating Action: %s | Cost: %f"), *Indent(depth), *action->Name, action->Cost);
		
		TSet<TSharedPtr<AgentBeliefs>> requiredDesiredEffects = parent->RequiredEffects;

		TArray<TSharedPtr<AgentBeliefs>> removeList;

		for (TSharedPtr<AgentBeliefs> belief : requiredDesiredEffects)
		{
			if (belief->Evaluate(inAI))
			{
				removeList.Add(belief);
			}
		}

		for (TSharedPtr<AgentBeliefs> belief : removeList)
		{
			requiredDesiredEffects.Remove(belief);
		}

		if (requiredDesiredEffects.Num() == 0)
		{
			//Debugging
			//UE_LOG(LogGOAP, Warning, TEXT("%sAll required effects satisfied at this node."), *Indent(depth));
			return true;
		}

		for (TSharedPtr<AgentBeliefs> belief : requiredDesiredEffects)
		{
			//Debugging
			bool bMatched = HasMatchingEffect(action->Effects, belief);
			if (!bMatched)
			{
				//UE_LOG(LogGOAP, Verbose, TEXT("%sAction %s does NOT satisfy belief %s"), *Indent(depth), *action->Name, *belief->Name);
			}
			
			
			//if (action->Effects.Contains(belief))
			if(HasMatchingEffect(action->Effects, belief))
			{
				//Debugging
				//UE_LOG(LogGOAP, Verbose, TEXT("%sAction %s does satisfy belief %s"), *Indent(depth), *action->Name, *belief->Name);
				
				TSet<TSharedPtr<AgentBeliefs>> newRequiredEffects = requiredDesiredEffects;
				newRequiredEffects = newRequiredEffects.Difference(action->Effects);
				
				newRequiredEffects = newRequiredEffects.Union(action->Preconditions);

				TSet<TSharedPtr<GoapAction>> newAvailableActions = actions;
				newAvailableActions.Remove(action);

				Node* newNode = new Node(parent, action, newRequiredEffects, parent->Cost + action->Cost);

				//Debugging
				//UE_LOG(LogGOAP, Warning, TEXT("%sRecursing with Action: %s | NewRequiredEffects: %d"), *Indent(depth), *action->Name, newRequiredEffects.Num());


				if (newRequiredEffects.Num() == 0)
				{
					parent->Leaves.Add(newNode);
					return true;
				}
				
				if (FindPath(newNode, inAI, newAvailableActions, depth + 1))
				{
					parent->Leaves.Add(newNode);
					return true;
					//newRequiredEffects.Difference(newNode->Action->Preconditions);
					//newRequiredEffects = newRequiredEffects.Difference(newNode->Action->Preconditions); ?
				}
				else
				{
					//UE_LOG(LogGOAP, Warning, TEXT("%sPath failed after Action: %s"), *Indent(depth), *action->Name);
				}
				
			}
		}
	}

	if (parent->Leaves.Num() == 0)
	{
		//UE_LOG(LogGOAP, Error, TEXT("%sNo valid leaves found. Path dead ends."), *Indent(depth));
	}

	return parent->Leaves.Num() > 0;
	//return false;
}

bool GoapPlanner::HasMatchingEffect(TSet<TSharedPtr<AgentBeliefs>>& actionEffects, TSharedPtr<AgentBeliefs> belief)
{
	for (TSharedPtr<AgentBeliefs> effect : actionEffects)
	{
		if (effect->Equals(belief))
		{
			return true;
		}
	}
	return false;
}

