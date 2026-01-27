// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseItem.h"

bool ABaseItem::Interact_Implementation(AActor* interactor)
{
	if (!interactor) return false;

	if (UInventoryComponent* inventory = interactor->FindComponentByClass<UInventoryComponent>())
	{
		if (inventory->PickUpItem(ItemType))
		{
			Destroy();
			return true;
		}
	}

	return false;
}
