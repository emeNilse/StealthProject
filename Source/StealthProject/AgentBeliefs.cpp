
#include "AgentBeliefs.h"

AgentBeliefs::AgentBeliefs(FString name)
{
	Name = name;
}

bool const AgentBeliefs::Evaluate(AAI_Controller* inAI)
{
	return ConditionFunc ? ConditionFunc() : false;
}

bool AgentBeliefs::Equals(TSharedPtr<AgentBeliefs> comparison)
{
	return (this->Name == comparison->Name);
}

AgentBeliefs::Builder::Builder(TSharedPtr<AgentBeliefs>& InBelief) : Belief(InBelief) {}