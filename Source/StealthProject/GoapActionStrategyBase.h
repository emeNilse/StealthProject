// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
//#include "IGoapActionStrategy.h"
#include "GoapActionStrategyBase.generated.h"

/**
 * 
 */
UCLASS(Abstract, Blueprintable)
class STEALTHPROJECT_API UGoapActionStrategyBase : public UObject
{
	GENERATED_BODY()

public:

	//virtual void Initialize() {}

	virtual void Start() {}

	virtual void Tick(float DeltaTime) {}

	virtual void Stop() {}

	virtual bool CanPerform() const { return true; }

	virtual bool Complete() const { return false; }

};

template<typename TStrategy, typename... Args>
TStrategy* MakeStrategy(UObject* Outer, Args... inputs)
{
	static_assert(TIsDerivedFrom<TStrategy, UGoapActionStrategyBase>::IsDerived, "TStrategy must derive from UGoapActionStrategyBase");

	TStrategy* Strategy = NewObject<TStrategy>(Outer);

	Strategy->Initialize(inputs...);

	return Strategy;
}
