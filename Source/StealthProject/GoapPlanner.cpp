

#include "GoapPlanner.h"
#include "GoapComponent.h"
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
		Node* startNode = new Node(nullptr, nullptr, goal->DesiredEffects, 0);
		startNode->HCost = Heuristic(startNode->RequiredEffects);

		Node* pathResult = FindPathAStar(startNode, inAI, agent->GetActions());
		
		if (!pathResult) continue;

		TArray<TSharedPtr<GoapAction>> actions = BuildPlan(pathResult);

		return MakeShared<ActionPlan>(goal, actions, pathResult->GCost);
		
		//Old path finder
		//Node* goalNode = new Node(nullptr, nullptr, goal->DesiredEffects, 0);
		//if (FindPath(goalNode, inAI, agent->Actions, 0))
		//{
		//	if (goalNode->IsLeafDead())
		//	{
		//		continue;
		//	}
		//	TArray<TSharedPtr<GoapAction>> goapActionStack;
		//	//Node* currentNode = goalNode;
		//	while (goalNode->Leaves.Num() > 0)
		//	{
		//		goalNode->Leaves.Sort([](const Node& A, const Node& B)
		//			{
		//				return A.Cost < B.Cost;
		//			});
		//		Node* cheapestLeaf = goalNode->Leaves[0];
		//		goalNode = cheapestLeaf;
		//		goapActionStack.Push(cheapestLeaf->Action);
		//	}
		//	//To make the plan a "stack"
		//	//Algo::Reverse(goapActionStack);
		//	return MakeShared<ActionPlan>(goal, goapActionStack, goalNode->Cost);
		//}
	}
	
	return nullptr;
}

bool GoapPlanner::FindPath(Node* parent, AAI_Controller* inAI, TSet<TSharedPtr<GoapAction>> actions, int32 depth)
{
	//Debugging
	//UE_LOG(LogGOAP, Warning, TEXT("%sEntering FindPath | Cost: %f | RequiredEffects: %d"), *Indent(depth), parent->Cost, parent->RequiredEffects.Num());
	
	TArray<TSharedPtr<GoapAction>> orderdActions = actions.Array();

	orderdActions.Sort([](const TSharedPtr<GoapAction>& A, const TSharedPtr<GoapAction>& B)
		{
			return A->CostValue() < B->CostValue();
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

				Node* newNode = new Node(parent, action, newRequiredEffects, parent->Cost + action->CostValue());

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
}

Node* GoapPlanner::FindPathAStar(Node* parentNode, AAI_Controller* inAI, TSet<TSharedPtr<GoapAction>> actions)
{
	TArray<Node*> openSet;
	TArray<Node*> closedSet;

	openSet.Add(parentNode);

	while (openSet.Num() > 0)
	{
		openSet.Sort([](Node& A, Node& B)
			{
				return A.FCost() < B.FCost();
			});

		Node* currentNode = openSet[0];
		openSet.RemoveAt(0);

		//Debugging
		//UE_LOG(LogGOAP, Warning, TEXT("Processing node | G: %f H: %f F: %f | RequiredEffects: %d | OpenSet: %d ClosedSet: %d"), currentNode->GCost, currentNode->HCost, currentNode->FCost(), currentNode->RequiredEffects.Num(), openSet.Num(), closedSet.Num());

		//Remove any Required Effects that are already true.
		TSet<TSharedPtr<AgentBeliefs>> requiredDesiredEffects = currentNode->RequiredEffects;
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

		//If Required Effects is empty, then all requirements are satisfied. Return current node and set up the action plan.
		if (requiredDesiredEffects.Num() == 0)
		{
			//Debugging
			/*UE_LOG(LogGOAP, Warning, TEXT("Goal reached! Final node cost: &f"), currentNode->GCost);
			Node* trace = currentNode;
			while (trace)
			{
				if (trace->Action)
				{
					UE_LOG(LogGOAP, Warning, TEXT("Step: %s"), *trace->Action->Name);
				}
				trace = trace->Parent;
			}*/

			return currentNode;
		}

		currentNode->RequiredEffects = requiredDesiredEffects;

		//Debugging
		/*UE_LOG(LogGOAP, Verbose, TEXT("After pruning, RequiredEffects: %d"), requiredDesiredEffects.Num());
		if (requiredDesiredEffects.Num() == 1)
		{
			TSharedPtr<AgentBeliefs> b = *requiredDesiredEffects.CreateIterator();
			
			UE_LOG(LogGOAP, Verbose, TEXT("The Required Effect: %s"), *b->Name);
		}*/

		closedSet.Add(currentNode);
		//Debugging
		//UE_LOG(LogGOAP, Verbose, TEXT("Moved node to ClosedSet. Closed size: %d"), closedSet.Num());

		for (TSharedPtr<GoapAction> action : actions)
		{
			//Debugging
			//UE_LOG(LogGOAP, Verbose, TEXT("Evaluating Actions: %s | Cost: %f"), *action->Name, action->CostValue());
			
			bool bUseful = false;
			for (TSharedPtr<AgentBeliefs> belief : currentNode->RequiredEffects)
			{
				if (HasMatchingEffect(action->Effects, belief))
				{
					bUseful = true;
					break;
				}
			}
			if (!bUseful)
			{
				//Debugging
				//UE_LOG(LogGOAP, Verbose, TEXT("Skipping Action %s (not useful)"), *action->Name);
				continue;
			}

			TSet<TSharedPtr<AgentBeliefs>> newEffects = currentNode->RequiredEffects;
			newEffects = newEffects.Difference(action->Effects);
			newEffects = newEffects.Union(action->Preconditions);

			float newG = currentNode->GCost + action->CostValue();
			float newH = Heuristic(newEffects);

			Node* neighbourNode = new Node(currentNode, action, newEffects, newG);
			neighbourNode->HCost = newH;

			//Debugging
			//UE_LOG(LogGOAP, Warning, TEXT("Creating neighbour via Action: %s | NewEffects: %d | G: %f H: %f F: %f"), *action->Name, newEffects.Num(), newG, newH, newG + newH);

			/*bool bVisited = false;
			for (Node* closed : closedSet)
			{
				if (closed->RequiredEffects.Includes(newEffects) && newEffects.Includes(closed->RequiredEffects))
				{
					bVisited = true;
					break;
				}
			}
			if (bVisited) continue;*/

			if (IsInClosedAndBetterCost(neighbourNode, closedSet))
			{
				//Debugging
				//UE_LOG(LogGOAP, Verbose, TEXT("Skipping neighbour (better node already in ClosedSet)"));
				continue;
			}
			openSet.Add(neighbourNode);
			//Debugging
			//UE_LOG(LogGOAP, Verbose, TEXT("Added neighbour to OpenSet. New size: %d"), openSet.Num());
		}
	}
	//Debugging
	//UE_LOG(LogGOAP, Error, TEXT("A* failed: OpenSet exhausted, no solution found."))
	return nullptr;
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

float GoapPlanner::Heuristic(const TSet<TSharedPtr<AgentBeliefs>>& effects)
{
	return effects.Num();
}

bool GoapPlanner::IsInClosedAndBetterCost(Node* node, const TArray<Node*>& ClosedList)
{
	for (Node* closedNode : ClosedList)
	{
		if (AreEffectsEqual(node->RequiredEffects, closedNode->RequiredEffects))
		{
			if (closedNode->GCost <= node->GCost)
			{
				return true;
			}
		}
	}

	return false;
}

bool GoapPlanner::AreEffectsEqual(TSet<TSharedPtr<AgentBeliefs>>& A, TSet<TSharedPtr<AgentBeliefs>>& B)
{
	if (A.Num() != B.Num())
		return false;

	for (const TSharedPtr<AgentBeliefs>& beliefA : A)
	{
		bool bFound = false;

		for (const TSharedPtr<AgentBeliefs>& beliefB : B)
		{
			if (beliefA->Equals(beliefB))
			{
				bFound = true;
				break;
			}
		}

		if (!bFound)
			return false;
	}

	return true;
}

TArray<TSharedPtr<GoapAction>> GoapPlanner::BuildPlan(Node* endNode)
{
	TArray<TSharedPtr<GoapAction>> plan;
	Node* current = endNode;

	while (current && current->Action)
	{
		plan.Push(current->Action);
		current = current->Parent;
	}
	Algo::Reverse(plan);
	return plan;
}

