// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameInstance.h"

UActionStack* UMyGameInstance::GetActionStack() const
{
	return GetSubsystem<UActionStack>();
}

UMyGameInstance::UMyGameInstance()
{
	static ConstructorHelpers::FClassFinder<UActionBehaviour> MainMenuBP(TEXT("/Game/ThirdPerson/Blueprints/UI/BP_MainMenuAction"));
	if (MainMenuBP.Succeeded())
	{
		StartupAction = MainMenuBP.Class;
	}
}

void UMyGameInstance::Init()
{
	Super::Init();

	UActionStack* Stack = GetSubsystem<UActionStack>();

	if (!Stack) return;

	//TSubclassOf<UActionBehaviour> MainMenuClass = StartupAction;

	if (!StartupAction)
	{
		UE_LOG(LogTemp, Warning, TEXT("StartupActionClass not found!"));
		return;
	}

	UObject* Action = NewObject<UObject>(this, StartupAction);
	if (!Action)
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to create startup action instance"));
		return;
	}

	Stack->PushAction(Action);
}
