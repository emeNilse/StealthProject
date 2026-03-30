
#pragma once

#include "CoreMinimal.h"
#include "GoapActionStrategyBase.h"
#include "SquadComponent.h"
#include "GetFlankPositionStrategy.generated.h"


UCLASS(Blueprintable, EditInlineNew)
class STEALTHPROJECT_API UGetFlankPositionStrategy : public UGoapActionStrategyBase
{
	GENERATED_BODY()
	
public: 

	virtual UGoapActionStrategyBase* CreateRunTimeInstance(UObject* outer, AAI_Controller* inAI) const override
	{
		UGetFlankPositionStrategy* Runtime = NewObject<UGetFlankPositionStrategy>(outer);
		Runtime->AI = inAI;

		return Runtime;
	}

	virtual void Start() override;

	virtual void Tick(float DeltaTime) override;

	virtual bool Complete() const override;

	void AcquiredFlankingposition();

private:

	AAI_Controller* AI = nullptr;

	bool bCalculationComplete = false;

	USquadComponent* squadComponent;
};
