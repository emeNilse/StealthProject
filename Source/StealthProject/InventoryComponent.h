// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InventoryComponent.generated.h"

UENUM(BlueprintType)
enum class EItemType : uint8
{
	None, 
	Resource,
	Supply,
	Fuel
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class STEALTHPROJECT_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UInventoryComponent();

	UFUNCTION(BlueprintPure)
	bool HasItem() const;

	UFUNCTION(BlueprintPure)
	EItemType GetHeldItem() const;

	bool PickUpItem(EItemType item);
	EItemType DropItem();


private:

	UPROPERTY()
	EItemType HeldItemType = EItemType::None;
};
