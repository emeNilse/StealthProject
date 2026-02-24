
#pragma once

#include "CoreMinimal.h"
#include "GoapBelief.h"
#include "BooleanBelief.generated.h"

//This is a Goap Belief that is based on a boolean as its belief.
//Normally these booleans are set to false, to imply that they can alway be executed.
//e.g. the belief AttackingPlayer (when I finally implement it), will never be satified and therefore 
//implying that the AI can run its Attack Strategy over and over again.

UCLASS(Blueprintable, EditInlineNew)
class STEALTHPROJECT_API UBooleanBelief : public UGoapBelief
{
	GENERATED_BODY()
	
public:

	UPROPERTY(EditAnywhere)
	bool bDeafaultValue = false;

	virtual bool Evaluate(AAI_Controller* AI) const override
	{
		return bDeafaultValue;
	}
};
