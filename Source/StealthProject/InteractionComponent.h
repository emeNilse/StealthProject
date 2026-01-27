// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Interactable.h"
#include "InteractionComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class STEALTHPROJECT_API UInteractionComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	
	UInteractionComponent();

	bool TryInteract(AActor* target);
		
};
