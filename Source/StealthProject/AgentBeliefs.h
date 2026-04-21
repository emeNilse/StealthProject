
#pragma once

#include "CoreMinimal.h"

//When I first started working on my Goap, I created this, the Agent Beliefs. 
//The idea was to somehow link Goap Beliefs to individual AIs. 

//This is where I got more acquainted with Shared Pointers and the excessive use of Lambda functions. 
//Agent Beliefs stores a belief name along with its condition for determining if the belief is true.

//As you can see below originally, before designing Goap Beliefs, my intention was that the Agent Belief would store all info
//related to a belief. 
//Halfway through I realized that Agent Beliefs was somewhat redundant. However, having had put so much time into it I chose to keep it.

//I have not deleted much of the original code (sue me) because at times I wanted to look back at how it originally worked, which
//did help me to understand the usage of shared pointers and how to set up Lambda functions.

//Goap Beliefs are created in the editor, the Goap Component registers them with the Belief Factory, which then stores the beliefs as
//AgentBeliefs in its BeliefRegistry (see bottom).

class AAI_Controller;

class STEALTHPROJECT_API AgentBeliefs 
{
	
public:
	FString Name;

	TFunction<bool()> ConditionFunc;
	TFunction<FVector()> ObservedLocation;

	bool const Evaluate(AAI_Controller* inAI);

	bool Equals(TSharedPtr<AgentBeliefs> comparison);

	FVector const GetLocation();

	AgentBeliefs(FString name);


	class Builder
	{
		TSharedPtr<AgentBeliefs> Belief;

	public:
		Builder(TSharedPtr<AgentBeliefs>& InBelief);
		//Builder(const FString name);
		//Builder(const TSharedPtr<AgentBeliefs>& name);

		//I don't understand why this is what works, I tried my own way first but a youtube example and chatgpt told me it had to be written like this
		Builder& WithCondition(TFunction<bool()> func)
		{
			Belief->ConditionFunc = func;
			return *this;
		}

		Builder& WithLocation(TFunction<FVector()> func)
		{
			Belief->ObservedLocation = func;
			return *this;
		}

		TSharedPtr<AgentBeliefs> Build()
		{
			return Belief;
		}

	};

	class BeliefRegistry
	{
	public:
		TSharedPtr<AgentBeliefs> Get(const FString& name)
		{
			if (!Cache.Contains(name))
			{
				Cache.Add(name, MakeShared<AgentBeliefs>(name));
			}

			return Cache[name];
		}

	private:
		TMap<FString, TSharedPtr<AgentBeliefs>> Cache;
	};
};


