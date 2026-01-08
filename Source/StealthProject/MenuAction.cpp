// Fill out your copyright notice in the Description page of Project Settings.


#include "MenuAction.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"

void UMenuAction::OnBegin_Implementation(bool bFirstTime)
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

		if (WidgetInstance->FindFunction(TEXT("SetOwningAction")))
		{
			struct { UActionBehaviour* Action; } Params;

			Params.Action = this;
			WidgetInstance->ProcessEvent(WidgetInstance->FindFunction(TEXT("SetOwningAction")), &Params);
		}
	}
}

void UMenuAction::OnEnd_Implementation()
{
	if (WidgetInstance)
	{
		WidgetInstance->RemoveFromParent();
		WidgetInstance = nullptr;
	}
}

bool UMenuAction::IsDone_Implementation() const
{
	return bIsDone;
}

void UMenuAction::RequestDone()
{
	bIsDone = true;
}
