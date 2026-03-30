
#pragma once

#include "CoreMinimal.h"
#include "GoapActionStrategyBase.h"
#include "NPC.h"
#include "ShootAtTargetStrategy.generated.h"


UCLASS(Blueprintable, EditInlineNew)
class STEALTHPROJECT_API UShootAtTargetStrategy : public UGoapActionStrategyBase
{
	GENERATED_BODY()
	
public:

	virtual UGoapActionStrategyBase* CreateRunTimeInstance(UObject* outer, AAI_Controller* inAI) const override
	{
		UShootAtTargetStrategy* Runtime = NewObject<UShootAtTargetStrategy>(outer);
		Runtime->AI = inAI;

		return Runtime;
	}

	virtual void Start() override;

	virtual void Tick(float DeltaTime) override;

	virtual bool Complete() const override;

private:

	AAI_Controller* AI = nullptr;

	AActor* Target = nullptr;

	ANPC* SelfReference = nullptr;

};
