
#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "AI_Controller.h"
#include "GoapActionStrategyBase.generated.h"

//All Goap Actions require a Strategy to follow/run.
//And all strategies are based on this script. When the editor creates a Goap Action in the editor, they will pick a strategy to go along with it.
//Upon registration in the Goap Component, strategies are created as instances (they can be made unique),
// and the CreateRunTimeInstance is called in each strategy that initializes the strategy and its variables.


UENUM()
enum class EStrategyStatus : uint8
{
	Running,
	Succeeded,
	Failed
};


UCLASS(Abstract, Blueprintable)
class STEALTHPROJECT_API UGoapActionStrategyBase : public UObject
{
	GENERATED_BODY()

public:

	virtual UGoapActionStrategyBase* CreateRunTimeInstance(UObject* Outer, class AAI_Controller* inAI) const PURE_VIRTUAL(UGoapActionStrategyBase::CreateRunTimeInstance, return nullptr;);


	EStrategyStatus Status = EStrategyStatus::Running;

	virtual void Start() {}

	virtual void Tick(float DeltaTime) {}

	virtual void Stop() {}

	//CanPerform was the original "check if the strategy can be executed", but later replaced with the EStrategyStatus
	//I have let it remain in place in most scripts because I'm wondering if I can still use it somehow.
	virtual bool CanPerform() const { return true; }

	virtual bool HasFailed() const { return false; }

	virtual bool Complete() const { return false; }

	virtual EStrategyStatus TickStatus() { return Status; }

	virtual float GetCost(AAI_Controller* inAI, float DefaultCost) const { return DefaultCost; }

};

//This was the original "create the strategies" method, no longer in use, but wanted to keep it around as a refernce. 
template<typename TStrategy, typename... Args>
TStrategy* MakeStrategy(UObject* Outer, Args... inputs)
{
	static_assert(TIsDerivedFrom<TStrategy, UGoapActionStrategyBase>::IsDerived, "TStrategy must derive from UGoapActionStrategyBase");

	TStrategy* Strategy = NewObject<TStrategy>(Outer);

	Strategy->Initialize(inputs...);

	return Strategy;
}
