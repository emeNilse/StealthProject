
#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GoapBelief.h"
#include "GoapBeliefSet.generated.h"

//Goap Belief Set is a part of the Goap Component where the designer sets the info for Goap Beliefs before it is registered as Agent Beliefs.

UCLASS(Blueprintable)
class STEALTHPROJECT_API UGoapBeliefSet : public UObject
{
	GENERATED_BODY()
	
public:

	UPROPERTY(EditAnywhere, Instanced)
	TArray<UGoapBelief*> Beliefs;

};
