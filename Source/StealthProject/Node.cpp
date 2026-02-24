
#include "Node.h"

Node::Node(Node* parent, TSharedPtr<GoapAction> action, TSet<TSharedPtr<AgentBeliefs>> effects, float cost)
{
	Parent = parent;
	Action = action;
	RequiredEffects = effects;
	Cost = cost;
}

