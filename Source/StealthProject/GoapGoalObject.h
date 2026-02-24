
#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GoapGoalObject.generated.h"

//Goap Goal Object is a part of the Goap Component where the designer sets the info for Goap Goals before it is registered.
//This is the same as GoapBeliefSet and GoapActionSet, but once again named things before I thought it through... and renaming scripts in Unreal is a bitch.

UCLASS(Blueprintable, EditInlineNew)
class STEALTHPROJECT_API UGoapGoalObject : public UObject
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Priority;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FString> DesiredEffects;

};
