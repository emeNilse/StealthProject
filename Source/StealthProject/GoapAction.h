
#pragma once

#include "CoreMinimal.h"
#include "AgentBeliefs.h"
#include "GoapActionStrategyBase.h"

//Goap Action was designed alongside Agent Belief, hence structure is simalar. 
//Like Agent Belief, the Goap Action comes with a Builder that "registers/creates" the action upon adding it to the Goap Component's
//list of available actions.
//It comes with the ability to set cost, preconditions (that match with agent beliefs), and effects (that match with agent beliefs).
//As you can see this script is messy, because it gave me much frustration upon design and I had to learn a lot about Shared Pointers and
//Lambdas here... don't want to delete all because I want to come back and see how things work (or used to work).

class AAI_Controller;

class STEALTHPROJECT_API GoapAction : public FGCObject
{
public:
	GoapAction(FString name);
	~GoapAction();

	FString Name;

	//float Cost; This was changed to adapt for the Dynamic Range Cost I created for Move Strategy
	//The lambda called in Goap Component is costly, I wonder if there's a better solution.
	TFunction<float()> GetCost;

	float CostValue() const
	{
		if (GetCost)
		{
			return GetCost();
		}
		return 0.f;
	}

	TSet<TSharedPtr<AgentBeliefs>> Preconditions;

	TSet<TSharedPtr<AgentBeliefs>> Effects;

	//GoapAction is not a UObject, UGoapActionStrategyBase is. To protect Strategy needed to make GoapAction : FCGObject and add reference below
	UGoapActionStrategyBase* Strategy;

	virtual void AddReferencedObjects(FReferenceCollector& Collector) override;
	virtual FString GetReferencerName() const override;
	
	//only initializes, must use tick to update
	//bool bComplete = Strategy.IsValid() ? Strategy->Complete() : false;

	void Start();

	//A manual tick function, needs to be called somewhere else
	void Tick(float deltaTime);

	bool IsDone();

	EStrategyStatus StatusCheck();

	void Stop();

	void EvaluateEffects(AAI_Controller* OwnerAI);

	class Builder
	{
		TSharedPtr<GoapAction> action;

	public:

		Builder(const FString name);

		Builder& WithCost(TFunction<float()> CostFunc)
		{
			action->GetCost = CostFunc;
			return *this;
		}

		Builder& WithStrategy(UGoapActionStrategyBase* strategy)
		{
			action->Strategy = strategy;
			return *this;
		}

		/*Builder& WithStrategy(TSharedPtr<UGoapActionStrategyBase> strategy)
		{
			action->Strategy = strategy;
			return *this;
		}*/

		Builder& AddPrecondition(TSharedPtr<AgentBeliefs> preconditionBelief)
		{
			//const FString precondition
			// AgentBeliefs::BeliefRegistry::Get(precondition)
			//TSet uses hashes, AgentBeliefs is user defined and not primitive like int32 or FString
			//problem solved by making TSets shared pointers or giving the agent beliefs a key and value for hash conversion
			//action->Preconditions.Add(MakeShared<AgentBeliefs>(precondition));
			action->Preconditions.Add(preconditionBelief);
			return *this;
		}

		/*Builder& AddPreconditions(const TArray<FString> preconditions)
		{
			for (const FString& p : preconditions)
			{
				AddPrecondition(p);
			}
			return *this;
		}*/

		Builder& AddEffect(TSharedPtr<AgentBeliefs> effectBelief)
		{
			action->Effects.Add(effectBelief);
			return *this;
		}

		/*Builder& AddEffects(const TArray<FString> effects)
		{
			for (const FString& e : effects)
			{
				AddEffect(e);
			}
			return *this;
		}*/

		TSharedPtr<GoapAction> Build()
		{
			return action;
		}
	};
};
