
#include "BaseItem.h"

bool ABaseItem::Interact_Implementation(AActor* interactor, EInteractionType type)
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
