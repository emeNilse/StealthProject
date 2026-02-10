// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AgentBeliefs.h"

/**
 * 
 */
class STEALTHPROJECT_API GoapGoal
{
	
public:
	GoapGoal(FString name);
	~GoapGoal();

	FString Name;

	//FORCEINLINE const FString& GetName() const { return Name; }

	int Priority;

	//FORCEINLINE float GetPriority() const { return Priority; }

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
