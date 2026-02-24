
#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GoapActionStrategyBase.h"
#include "GoapActionsSet.generated.h"

//Goap Action Set is a part of the Goap Component where the designer sets the info for a Goap Action before it is created.

UCLASS(Blueprintable, EditInlineNew)
class STEALTHPROJECT_API UGoapActionsSet : public UObject
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Cost;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FString> PreConditions;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FString> Effects;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector Location;

	UPROPERTY(EditAnywhere, Instanced, BlueprintReadWrite)
	UGoapActionStrategyBase* StrategyInstance;

	float GetDynamicCost(AAI_Controller* inAI) const
	{
		if (StrategyInstance)
		{
			return StrategyInstance->GetCost(inAI, Cost);
		}
		return Cost;
	}
};
