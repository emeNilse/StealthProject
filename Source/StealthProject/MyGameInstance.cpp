
#include "MyGameInstance.h"
#include "ActionBehaviour.h"
#include "Kismet/GameplayStatics.h"

UActionStack* UMyGameInstance::GetActionStack() const
{
	return GetSubsystem<UActionStack>();
}

void UMyGameInstance::HandlePostLoadMap(UWorld* loadedLevel)
{
	if (!loadedLevel)
	{
		return;
	}

	if (loadedLevel->IsGameWorld())
	{
		//Stack->ClearStack();
	}
}

void UMyGameInstance::TryPushStartupAction(UWorld* World, ELevelTick TickType, float DeltaSeconds)
{
	if (bStartupActionPushed || !World || !Stack) return;

	APlayerController* PC = World->GetFirstPlayerController();
	if (!PC) return; //PC not ready yet, wait for next tick

	//Create and initialize main menu action
	UActionBehaviour* Action = NewObject<UActionBehaviour>(this, StartupAction);
	Action->Initialize(PC);

	//Push it onto the stack
	Stack->PushAction(Action);

	bStartupActionPushed = true;

	//Remove delegate so it doesn't run again
	FWorldDelegates::OnWorldPostActorTick.RemoveAll(this);
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

	FCoreUObjectDelegates::PostLoadMapWithWorld.AddUObject(this, &UMyGameInstance::HandlePostLoadMap);
	FWorldDelegates::OnWorldPostActorTick.AddUObject(this, &UMyGameInstance::TryPushStartupAction);

	Stack = GetSubsystem<UActionStack>();

	if (!Stack) return;

	if (!StartupAction)
	{
		UE_LOG(LogTemp, Warning, TEXT("StartupActionClass not found!"));
		return;
	}
}
