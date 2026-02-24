
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interactable.h"
#include "InventoryComponent.h"
#include "BaseItem.generated.h"

//Base Item is currently not in use.
//The plan is to, after the game is handed in, to let the AI actually pick up items and have a basic inventory.

UCLASS()
class STEALTHPROJECT_API ABaseItem : public AActor, public IInteractable
{
	GENERATED_BODY()
	
public:	
	UPROPERTY(EditAnywhere)
	EItemType ItemType;

	virtual bool Interact_Implementation(AActor* interactor, EInteractionType type) override;
};
