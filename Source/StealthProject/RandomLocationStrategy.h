// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GoapActionStrategyBase.h"
#include "NavigationSystem.h"
#include "AI_Controller.h"
#include "NavigationSystem.h"
#include "Navigation/PathFollowingComponent.h"
#include "NavigationPath.h"
#include "NPC.h"
#include "RandomLocationStrategy.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, EditInlineNew)
class STEALTHPROJECT_API URandomLocationStrategy : public UGoapActionStrategyBase
{
	GENERATED_BODY()
	
public:
	virtual UGoapActionStrategyBase* CreateRunTimeInstance(UObject* Outer, AAI_Controller* inAI) const override
	{
		URandomLocationStrategy* Runtime = NewObject<URandomLocationStrategy>(Outer);
		Runtime->AI = inAI;
		Runtime->SearchRadius = SearchRadius;
		Runtime->WithinMinimumRange = WithinMinimumRange;
		Runtime->MaxStuckTime = MaxStuckTime;
		return Runtime;
	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SearchRadius = 1500.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool DynamicRangeCostActive = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float WithinMinimumRange;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxStuckTime = 3.f;

	virtual void Start() override;

	virtual void Tick(float DeltaTime) override;

	virtual void Stop() override;

	virtual bool CanPerform() const override { return !Complete(); };

	virtual bool Complete() const override;

	virtual float GetCost(AAI_Controller* inAI, float DefaultCost) const override;

	float GetRemainingDistance(AAI_Controller* inAI, const FVector& targetDestination) const;

private:
	AAI_Controller* AI;

	FVector Destination;

	float StuckTimer;
};
