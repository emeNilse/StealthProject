
#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Interactable.generated.h"

//Interactable interface for any interactable items and stations

UENUM()
enum class EInteractionStatus : uint8
{
	InProgress,
	Succeeded,
	Failed
};

UENUM()
enum class EInteractionType : uint8
{
	Default,
	Storage_Check,
	Storage_Take,
	Storage_Refill
};

UINTERFACE(MinimalAPI)
class UInteractable : public UInterface
{
	GENERATED_BODY()
};


class STEALTHPROJECT_API IInteractable
{
	GENERATED_BODY()


public:

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	bool Interact(AActor* interactor, EInteractionType type);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	bool IsInteractionComplete();
};
