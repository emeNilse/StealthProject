
#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "AI_Controller.h"
#include "GoapBelief.generated.h"

//Goap Belief is the base class for the actual beliefs given to an AI in the editor.
//Any and all beliefs that can be given to the AI stem from here.
//Goap Component has a TSet of Goap Beliefs, wherein a designer may add and create as many beliefs to their hearts content,
//and is registered upon construction.
//The evaluate function is overwritten in each Belief to match the evaluation requirements.

UENUM(BlueprintType)
enum class EFloatComparison : uint8
{
	Less,
	LessOrEqual,
	Greater,
	GreaterOrEqual,
	Equal,
	Range
};

UCLASS(Abstract, Blueprintable, EditInlineNew)
class STEALTHPROJECT_API UGoapBelief : public UObject
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FString Name;

	virtual bool Evaluate(class AAI_Controller* AI) const PURE_VIRTUAL(UGoapBelief::Evaluate, return false;);
};
