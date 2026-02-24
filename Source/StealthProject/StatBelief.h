
#pragma once

#include "CoreMinimal.h"
#include "GoapBelief.h"
#include "NPC.h"
#include "StatBelief.generated.h"

//Goap Belief that checks the AI's stats.

UCLASS(Blueprintable, EditInlineNew)
class STEALTHPROJECT_API UStatBelief : public UGoapBelief
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere)
	FName StatName;

	UPROPERTY(EditAnywhere)
	EFloatComparison Comparison = EFloatComparison::Less;

	UPROPERTY(EditAnywhere)
	float Value = 0.f;

	UPROPERTY(EditAnywhere)
	float MaxRangeValue = 0.f;

	const float GetStatValue(APawn* pawn) const;

	bool Compare(float stat) const;

	virtual bool Evaluate(AAI_Controller* AI) const override
	{
		APawn* Pawn = AI->GetPawn();
		if (!Pawn) return false;

		const float Stat = GetStatValue(Pawn);
		if (!FMath::IsFinite(Stat)) return false;
		
		return Compare(Stat);
	}
};
