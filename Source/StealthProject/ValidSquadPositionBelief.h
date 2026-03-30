
#pragma once

#include "CoreMinimal.h"
#include "GoapBelief.h"
#include "ValidSquadPositionBelief.generated.h"


UCLASS(Blueprintable, EditInlineNew)
class STEALTHPROJECT_API UValidSquadPositionBelief : public UGoapBelief
{
	GENERATED_BODY()
	
public: 

	UPROPERTY(EditAnywhere)
	FName BlackboardKey;

	virtual bool Evaluate(AAI_Controller* AI) const override;

};
