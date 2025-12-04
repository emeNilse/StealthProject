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

	// const FString& GetName() const { return Name; }
	
	// float GetCost() const { return Cost; }

	TSet<TSharedPtr<AgentBeliefs>> Preconditions;

	TSet<TSharedPtr<AgentBeliefs>> Effects;

	TSharedPtr<IGoapActionStrategy> Strategy;
	
	bool bComplete = Strategy.IsValid() ? Strategy->Complete() : false;

	void Start();

	//A manual tick function, needs to be called somewhere else
	void Tick(float deltaTime);

	void Stop();

	class Builder
	{
		TSharedPtr<GoapAction> action;

	public:

		Builder(const FString name);

		Builder& WithCost(float cost)
		{
			action->Cost = cost;
			return *this;
		}

		Builder& WithStrategy(TSharedPtr<IGoapActionStrategy> strategy)
		{
			action->Strategy = strategy;
			return *this;
		}

		Builder& AddPrecondition(FString precondition)
		{
			//TSet uses hashes, AgentBeliefs is user defined and not primitive like int32 or FString
			//problem solved by making TSets shared pointers or giving the agent beliefs a key and value for hash conversion
			//action->Preconditions.Add(MakeShared<AgentBeliefs>(precondition));
			action->Preconditions.Add(AgentBeliefs::BeliefRegistry::Get(precondition));
			return *this;
		}

		Builder& AddEffect(FString effect)
		{
			action->Effects.Add(AgentBeliefs::BeliefRegistry::Get(effect));
			return *this;
		}

		TSharedPtr<GoapAction> Build()
		{
			return action;
		}
			

	};

private:

	//FORCEINLINE void SetCost(float cost) { Cost = cost; }

};
