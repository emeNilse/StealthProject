
#pragma once

#include "CoreMinimal.h"
#include "GoapBelief.h"
#include "GoapWorldStateComponent.h"
#include "GoapComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "WorldStateFactBelief.generated.h"

//Goap Belief that is meant to verify info from the World Facts.
//Have yet to find a suitable use for it. Coming in Goap Presentation.

UCLASS(Blueprintable, EditInlineNew)
class STEALTHPROJECT_API UWorldStateFactBelief : public UGoapBelief
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	FName FactName;

	UPROPERTY(EditAnywhere)
	EWorldFactType Type = EWorldFactType::Bool;

	UPROPERTY(EditAnywhere, meta = (EditCondition = "Type == EWorldFactType::Bool", EditConditionHides))
	bool FactBool;

	UPROPERTY(EditAnywhere, meta = (EditCondition = "Type == EWorldFactType::Int", EditConditionHides))
	int IntValue;

	UPROPERTY(EditAnywhere, meta = (EditCondition = "Type == EWorldFactType::Float", EditConditionHides))
	float FloatValue;

	UPROPERTY(EditAnywhere)
	float MaxRangeSquared;

	UPROPERTY(EditAnywhere)
	EFloatComparison Comparison = EFloatComparison::Less;

	UPROPERTY(EditAnywhere)
	FName BlackboardKey;

	bool CompareInt(int stat) const;

	bool CompareFloat(float stat) const;

	virtual bool Evaluate(AAI_Controller* AI) const override;
	
};
