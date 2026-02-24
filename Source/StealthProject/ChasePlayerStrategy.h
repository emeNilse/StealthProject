
#pragma once

#include "CoreMinimal.h"
#include "GoapActionStrategyBase.h"
#include "AI_Controller.h"
#include "StealthProjectCharacter.h"
#include "ChasePlayerStrategy.generated.h"

//This Strategy is for chaisng the player

UCLASS(Blueprintable, EditInlineNew)
class STEALTHPROJECT_API UChasePlayerStrategy : public UGoapActionStrategyBase
{
	GENERATED_BODY()

public:
	virtual UGoapActionStrategyBase* CreateRunTimeInstance(UObject* Outer, AAI_Controller* inAI) const override
	{
		UChasePlayerStrategy* Runtime = NewObject<UChasePlayerStrategy>(Outer);
		Runtime->AI = inAI;
		return Runtime;
	}

	AAI_Controller* AI;

	AStealthProjectCharacter* Player;

	virtual void Start() override;

	virtual void Tick(float DeltaTime) override;

	virtual void Stop() override;

	virtual bool CanPerform() const override;

	virtual bool Complete() const override;

	float GetRemainingDistance(AAI_Controller* inAI, AStealthProjectCharacter* inPlayer) const;

};
