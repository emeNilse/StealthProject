
#pragma once

#include "CoreMinimal.h"
#include "GoapActionStrategyBase.h"
#include "AimAtTargetStrategy.generated.h"


UCLASS(Blueprintable, EditInlineNew)
class STEALTHPROJECT_API UAimAtTargetStrategy : public UGoapActionStrategyBase
{
	GENERATED_BODY()
	
public:

	virtual UGoapActionStrategyBase* CreateRunTimeInstance(UObject* outer, AAI_Controller* inAI) const override
	{
		UAimAtTargetStrategy* Runtime = NewObject<UAimAtTargetStrategy>(outer);
		Runtime->AI = inAI;

		return Runtime;
	}

	virtual void Start() override;

	virtual void Tick(float DeltaTime) override;

	virtual bool Complete() const override;

private:

	AAI_Controller* AI = nullptr;
};
