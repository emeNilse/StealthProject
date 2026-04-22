
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InventoryComponent.generated.h"

//Inventory Component is not in use yet, coming soon.

UENUM(BlueprintType)
enum class EItemType : uint8
{
	None, 
	Resource,
	Supply,
	Fuel, 
	Weapon
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class STEALTHPROJECT_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UInventoryComponent();

	UFUNCTION(BlueprintPure)
	bool HasItem() const;

	UFUNCTION(BlueprintPure)
	EItemType GetHeldItem() const;

	UFUNCTION(BlueprintCallable)
	bool PickUpItem(EItemType item);
	EItemType DropItem();


private:

	UPROPERTY()
	EItemType HeldItemType = EItemType::None;
};
