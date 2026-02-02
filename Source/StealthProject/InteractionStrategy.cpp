// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractionStrategy.h"
#include "WorldStateSubsystem.h"

void UInteractionStrategy::Initialize(AAI_Controller* inIA, AActor* inTarget)
{
	AI = inIA;
	Target = inTarget;
}

void UInteractionStrategy::Start()
{
	if (!Target || !Target->Implements<UInteractable>())
	{
		return;
	}

	bInteractionResult = IInteractable::Execute_Interact(Target, AI->GetPawn(), InteractionType);
}

bool UInteractionStrategy::CanPerform() const
{
	return Target && Target->Implements<UInteractable>();
}

bool UInteractionStrategy::Complete() const
{
	//need to improve this
	if (!bInteractionResult)
	{
		return true;
	}

	return IInteractable::Execute_IsInteractionComplete(Target);
}
