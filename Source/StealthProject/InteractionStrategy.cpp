// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractionStrategy.h"
#include "GoapWorldStateComponent.h"

void UInteractionStrategy::Initialize(AAI_Controller* inIA, AActor* inTarget)
{
	AI = inIA;
	Target = inTarget;
}

void UInteractionStrategy::Start()
{
	if (!Target || !Target->Implements<UInteractable>())
	{
		Status = EStrategyStatus::Failed;
		return;
	}

	bInteractionResult = IInteractable::Execute_Interact(Target, AI->GetPawn(), InteractionType);

	if (!bInteractionResult)
	{
		Status = EStrategyStatus::Failed;
	}
	else
	{
		Status = EStrategyStatus::Running;
	}
}

void UInteractionStrategy::Tick(float DeltaTime)
{
	if (Complete())
	{
		Status = EStrategyStatus::Succeeded;
	}
}

bool UInteractionStrategy::CanPerform() const
{
	return Target && Target->Implements<UInteractable>();
}

bool UInteractionStrategy::Complete() const
{
	return IInteractable::Execute_IsInteractionComplete(Target);
}
