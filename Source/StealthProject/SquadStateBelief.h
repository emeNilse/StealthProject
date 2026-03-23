
#pragma once

#include "CoreMinimal.h"
#include "GoapBelief.h"
#include "SquadComponent.h"
#include "SquadStateBelief.generated.h"


UCLASS(Blueprintable, EditInlineNew)
class STEALTHPROJECT_API USquadStateBelief : public UGoapBelief
{
	GENERATED_BODY()
	
public:

	UPROPERTY(EditAnywhere)
	FName BlackboardKey;

	UPROPERTY(EditAnywhere)
	ESquadState DesiredSquadState;

	virtual bool Evaluate(AAI_Controller* AI) const override;
};
