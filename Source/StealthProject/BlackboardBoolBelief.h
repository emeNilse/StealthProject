
#pragma once

#include "CoreMinimal.h"
#include "GoapBelief.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AI_Controller.h"
#include "BlackboardBoolBelief.generated.h"

//This is a Goap Belief that looks for a Blackboard boolean as its belief.

UCLASS(Blueprintable, EditInlineNew)
class STEALTHPROJECT_API UBlackboardBoolBelief : public UGoapBelief
{
	GENERATED_BODY()
	
public:

	UPROPERTY(EditAnywhere)
	FName BlackboardKey;

	virtual bool Evaluate(AAI_Controller* AI) const override
	{
		if (!AI) return false;

		return AI->GetBlackboardComponent()->GetValueAsBool(BlackboardKey);
	}
};
