// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AgentBeliefs.h"
#include "IGoapActionStrategy.h"

/**
 * 
 */
class STEALTHPROJECT_API GoapAction
{
public:
	GoapAction(FString name);
	~GoapAction();

	FString Name;

	float Cost;

	//FORCEINLINE const FString& GetName() const { return Name; }
	
	//FORCEINLINE float GetCost() const { return Cost; }

	TSet<TSharedPtr<AgentBeliefs>> Preconditions;

	TSet<TSharedPtr<AgentBeliefs>> Effects;

	IGoapActionStrategy* Strategy;
	
	bool complete = Strategy->Complete();

	void Start();

	//A manual tick function, needs to be called somewhere else
	void Tick(float deltaTime);

	void Stop();

	class Builder
	{
		GoapAction* action;

	public:

		Builder(const FString name);

		Builder& WithCost(float cost)
		{
			action->Cost = cost;
			return *this;
		}

		Builder& WithStrategy(IGoapActionStrategy* strategy)
		{
			action->Strategy = strategy;
			return *this;
		}

		Builder& AddPrecondition(AgentBeliefs precondition)
		{
			//TSet uses hashes, AgentBeliefs is user defined and not primitive like int32 or FString
			//problem solved by making TSets shared pointers or giving the agent beliefs a key and value for hash conversion
			action->Preconditions.Add(MakeShared<AgentBeliefs>(precondition));
			return *this;
		}

		Builder& AddEffect(AgentBeliefs effect)
		{
			action->Effects.Add(MakeShared<AgentBeliefs>(effect));
			return *this;
		}

		GoapAction* Build()
		{
			return action;
		}
			

	};

private:

	//FORCEINLINE void SetCost(float cost) { Cost = cost; }

};
