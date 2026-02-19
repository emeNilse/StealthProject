// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GoapActionStrategyBase.h"
#include "AI_Controller.h"
#include "MoveStrategy.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, EditInlineNew)
class STEALTHPROJECT_API UMoveStrategy : public UGoapActionStrategyBase
{
	GENERATED_BODY()

public:
	
	virtual UGoapActionStrategyBase* CreateRunTimeInstance(UObject* Outer, AAI_Controller* inAI) const override
	{
		UMoveStrategy* Runtime = NewObject<UMoveStrategy>(Outer);
		Runtime->TargetActor = TargetActor;
		Runtime->AI = inAI;
		Runtime->WithinMinimumRange = WithinMinimumRange;
		return Runtime;
	}
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftObjectPtr <AActor> TargetActor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool DynamicRangeCostActive = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float WithinMinimumRange;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxStuckTime = 3.f;

	FVector Destination;

	//TFunction was for the pure C++ Goap, not supported by "Unreal's reflection system"(?)
	//TFunction<FVector()> Destination;

	AAI_Controller* AI;

	//virtual void Initialize(AAI_Controller* inAI, TSoftObjectPtr<AActor> inActor);

	virtual void Start() override;

	virtual void Tick(float DeltaTime) override;

	virtual void Stop() override;

	virtual bool CanPerform() const override;
	virtual bool Complete() const override;

	virtual float GetCost(AAI_Controller* inAI, float DefaultCost) const override;

	float GetRemainingDistance(AAI_Controller* inAI, const FVector& targetDestination) const;

private:

	float StuckTimer;
};
