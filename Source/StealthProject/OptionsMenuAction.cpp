// Fill out your copyright notice in the Description page of Project Settings.


#include "OptionsMenuAction.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"

void UOptionsMenuAction::OnBegin_Implementation(bool bFirstTime)
{
	if (!WidgetInstance && WidgetClass)
	{
		UWorld* World = GetWorld();
		if (!World) return;

		WidgetInstance = CreateWidget<UUserWidget>(World, WidgetClass);

		if (WidgetInstance)
		{
			WidgetInstance->AddToViewport(100);
			UGameplayStatics::GetPlayerController(World, 0)->SetInputMode(FInputModeUIOnly());
		}

		
	}
}

void UOptionsMenuAction::OnEnd_Implementation()
{
	if (WidgetInstance)
	{
		WidgetInstance->RemoveFromParent();
		WidgetInstance = nullptr;
	}
}

bool UOptionsMenuAction::IsDone_Implementation() const
{
	return bIsDone;
}
