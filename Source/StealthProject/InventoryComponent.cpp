// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryComponent.h"

// Sets default values for this component's properties
UInventoryComponent::UInventoryComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	//PrimaryComponentTick.bCanEverTick = true;
}

bool UInventoryComponent::HasItem() const
{
	return HeldItemType != EItemType::None;
}

EItemType UInventoryComponent::GetHeldItem() const
{
	return HeldItemType;
}

bool UInventoryComponent::PickUpItem(EItemType item)
{
	if (HeldItemType != EItemType::None) return false;

	HeldItemType = item;
	return true;
}

EItemType UInventoryComponent::DropItem()
{
	EItemType DroppedItem = HeldItemType;
	HeldItemType = EItemType::None;
	return DroppedItem;
}




