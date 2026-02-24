

#include "InventoryComponent.h"

UInventoryComponent::UInventoryComponent()
{
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




