
#pragma once

#include "CoreMinimal.h"
#include "AgentBeliefs.h"

//The Belief Factory gets created in the Goap Component, and is responsible for registering beliefs that have been 
//created in the editor as Agent Beliefs.

class USensor;
class UGoapComponent;

class STEALTHPROJECT_API BeliefFactory
{
public:
	BeliefFactory(UGoapComponent* component, AgentBeliefs::BeliefRegistry& registry);

	~BeliefFactory();

	void AddBelief(FString key, TFunction<bool()> condition);

private:
	UGoapComponent* Component;

	AgentBeliefs::BeliefRegistry& Registry;
};
