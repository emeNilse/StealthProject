// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interactable.h"
#include "InventoryComponent.h"
#include "BaseItem.generated.h"

UCLASS()
class STEALTHPROJECT_API ABaseItem : public AActor, public IInteractable
{
	GENERATED_BODY()
	
public:	
	UPROPERTY(EditAnywhere)
	EItemType ItemType;

	virtual bool Interact_Implementation(AActor* interactor, EInteractionType type) override;
};
