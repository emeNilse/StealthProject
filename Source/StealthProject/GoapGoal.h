
#pragma once

#include "CoreMinimal.h"
#include "AgentBeliefs.h"

//The Goap Goals, a desire that is based off an existing (Goap) Agent Belief

class STEALTHPROJECT_API GoapGoal
{
	
public:
	GoapGoal(FString name);
	~GoapGoal();

	FString Name;

	int Priority;

	TSet<TSharedPtr<AgentBeliefs>> DesiredEffects;

	class Builder
	{
		TSharedPtr<GoapGoal> goal;

	public:
		Builder(const FString name);

		Builder& WithPriority(int priority)
		{
			goal->Priority = priority;
			return *this;
		}

		Builder& WithDesiredEffect(TSharedPtr<AgentBeliefs> effect)
		{
			goal->DesiredEffects.Add(effect);
			return *this;
		}

		/*Builder& WithDesiredEffect(TArray<FString> effect)
		{
			for (FString e : effect)
			{
				goal->DesiredEffects.Add(AgentBeliefs::BeliefRegistry::Get(e));
			}
			return *this;
		}*/

		TSharedPtr<GoapGoal> Build()
		{
			return goal;
		}
	};
};
