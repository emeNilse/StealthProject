// Fill out your copyright notice in the Description page of Project Settings.


#include "MenuAction.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"

void UMenuAction::OnBegin_Implementation(bool bFirstTime)
{
	if (!WidgetInstance && WidgetClass)
	{
		UWorld* Worlds = GetWorld();
		UWorld* World = GetTypedOuter<UWorld>();
		if (!Worlds) return;

		//APlayerController* PC = OwningPlayerController;
		if (!OwningPlayerController) return;

		WidgetInstance = CreateWidget<UUserWidget>(OwningPlayerController, WidgetClass);

		if (WidgetInstance)
		{
			WidgetInstance->AddToViewport(100);
			OwningPlayerController->SetInputMode(FInputModeUIOnly());
			OwningPlayerController->bShowMouseCursor = true;
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

	if (OwningPlayerController)
	{
		//OwningPlayerController->SetInputMode(FInputModeGameOnly());
		//OwningPlayerController->bShowMouseCursor = false;
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
