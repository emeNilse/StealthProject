
#pragma once

#include "CoreMinimal.h"
#include "GoapBelief.h"
#include "NPC.h"
#include "NPCStateBelief.generated.h"

//Goap Belief that checks for the NPC's State

UCLASS(Blueprintable, EditInlineNew)
class STEALTHPROJECT_API UNPCStateBelief : public UGoapBelief
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere)
	ENPCState DesiredState;

	virtual bool Evaluate(AAI_Controller* AI) const override
	{
		if (!AI) return false;

		APawn* Pawn = AI->GetPawn();
		if (!Pawn) return false;

		ANPC* NPC = Cast<ANPC>(Pawn);
		if (!NPC) return false;

		return NPC->GetNPCState() == DesiredState;
	}
};
