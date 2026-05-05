
#include "Node.h"

Node::Node(Node* parent, TSharedPtr<GoapAction> action, TSet<TSharedPtr<AgentBeliefs>> effects, float cost)
{
	Parent = parent;
	Action = action;
	RequiredEffects = effects;
	GCost = cost;
}

Node::~Node()
{
	for (Node* n : Leaves)
	{
		if (n)
		{
			delete n;
		}
	}
	Leaves.Empty();
}
