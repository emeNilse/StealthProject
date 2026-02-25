

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

		if (!OwningPlayerController) return;

		WidgetInstance = CreateWidget<UUserWidget>(OwningPlayerController, WidgetClass);

		if (WidgetInstance)
		{
			WidgetInstance->AddToViewport(100);
			WidgetInstance->SetVisibility(ESlateVisibility::Collapsed);
		}

		if (WidgetInstance->FindFunction(TEXT("SetOwningAction")))
		{
			struct { UActionBehaviour* Action; } Params;

			Params.Action = this;
			WidgetInstance->ProcessEvent(WidgetInstance->FindFunction(TEXT("SetOwningAction")), &Params);
		}
	}
}

void UMenuAction::OnUpdate_Implementation()
{
	if (!WidgetInstance) return;

	if (!bIsActive)
	{
		bIsActive = true;

		WidgetInstance->SetVisibility(ESlateVisibility::Visible);

		if (OwningPlayerController)
		{
			OwningPlayerController->SetInputMode(FInputModeUIOnly());
			OwningPlayerController->bShowMouseCursor = true;
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

void UMenuAction::DeactivateWidget()
{
	if (!WidgetInstance) return;

	bIsActive = false;
	WidgetInstance->SetVisibility(ESlateVisibility::Collapsed);
}
