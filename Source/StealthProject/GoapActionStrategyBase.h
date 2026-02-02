// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
//#include "IGoapActionStrategy.h"
#include "GoapActionStrategyBase.generated.h"

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

	//Why is this not allowed?? All strategies need an initialize, but heaven forbid, all inheritance logic demands they have their own... 
	//virtual void Initialize() {}

	virtual void Start() {}

	virtual void Tick(float DeltaTime) {}

	virtual void Stop() {}

	virtual bool CanPerform() const { return true; }

	virtual bool Complete() const { return false; }

	virtual EStrategyStatus TickStatus() { return EStrategyStatus::Running; }

};

template<typename TStrategy, typename... Args>
TStrategy* MakeStrategy(UObject* Outer, Args... inputs)
{
	static_assert(TIsDerivedFrom<TStrategy, UGoapActionStrategyBase>::IsDerived, "TStrategy must derive from UGoapActionStrategyBase");

	TStrategy* Strategy = NewObject<TStrategy>(Outer);

	Strategy->Initialize(inputs...);

	return Strategy;
}
