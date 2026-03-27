
#pragma once

#include "CoreMinimal.h"
#include "GoapBelief.h"
#include "SquadComponent.h"
#include "SquadRoleBelief.generated.h"


UCLASS(Blueprintable, EditInlineNew)
class STEALTHPROJECT_API USquadRoleBelief : public UGoapBelief
{
	GENERATED_BODY()
	
public:

	UPROPERTY(EditAnywhere)
	ESquadRole DesiredSquadRole;

	virtual bool Evaluate(AAI_Controller* AI) const override;
};
