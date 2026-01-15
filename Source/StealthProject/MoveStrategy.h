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
	/*MoveStrategy(AAI_Controller* inAI, TFunction<FVector()> inDestination);
	~MoveStrategy();*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector Destination;

	//TFunction was for the pure C++ Goap, not supported by "Unreal's reflection system"(?)
	//TFunction<FVector()> Destination;

	AAI_Controller* AI;

	virtual void Initialize(AAI_Controller* inAI, FVector inDestination);

	virtual void Start() override;

	virtual void Tick(float DeltaTime) override;

	virtual void Stop() override;

	virtual bool CanPerform() const override;

	virtual bool Complete() const override;

	float GetRemainingDistance(AAI_Controller* inAI, const FVector& targetDestination) const;
};
