
#pragma once

#include "CoreMinimal.h"
#include "GoapActionStrategyBase.h"
#include "AI_Controller.h"
#include "Interactable.h"
#include "InteractionStrategy.generated.h"

//Strategy that let the AI call the station/items Interact function.

UCLASS(Blueprintable, EditInlineNew)
class STEALTHPROJECT_API UInteractionStrategy : public UGoapActionStrategyBase
{
	GENERATED_BODY()

public:
	virtual UGoapActionStrategyBase* CreateRunTimeInstance(UObject* Outer, AAI_Controller* inAI) const override
	{
		UInteractionStrategy* Runtime = NewObject<UInteractionStrategy>(Outer);
		Runtime->AI = inAI;
		Runtime->Target = Target;
		Runtime->InteractionType = InteractionType;
		return Runtime;
	}
	
	UPROPERTY(EditAnywhere)
	AActor* Target;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EInteractionType InteractionType;

	virtual void Start() override;

	virtual void Tick(float DeltaTime) override;

	virtual bool CanPerform() const override;

	virtual bool Complete() const override;


private:
	AAI_Controller* AI = nullptr;

	bool bInteractionResult;
};
