#pragma once
#include "CoreMinimal.h"
#include "GoapBelief.h"
#include "GoapComponent.h"
#include "BasedOnOtherBooleansBelief.generated.h"


UCLASS(Blueprintable, EditInlineNew)
class STEALTHPROJECT_API UBasedOnOtherBooleansBelief : public UGoapBelief
{
	GENERATED_BODY()
	
public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bCheckIfTrue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FString> BeliefNames;

	virtual bool Evaluate(AAI_Controller* AI) const override
	{
		if (!AI) return false;

		for (const FString& beliefName : BeliefNames)
		{
			TSharedPtr<AgentBeliefs> b = AI->GetGoap()->GetBeliefFromRegistry(beliefName);
			if (b->Evaluate(AI) && bCheckIfTrue)
			{
				return true;
			}
			else if (!b->Evaluate(AI) && !bCheckIfTrue)
			{
				return true;
			}
		}
		
		return false;
	}
};
