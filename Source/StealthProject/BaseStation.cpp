
#include "BaseStation.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"
#include "Camera/PlayerCameraManager.h"
#include "WorldFactRegistry.h"

ABaseStation::ABaseStation()
{
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	TextRenderer = CreateDefaultSubobject<UTextRenderComponent>(TEXT("TextRenderer"));
	TextRenderer->SetupAttachment(RootComponent);

	DisplayName = TEXT("Station name");
}

bool ABaseStation::Interact_Implementation(AActor* interactor, EInteractionType type)
{
	UE_LOG(LogTemp, Warning, TEXT("Base station interaction called"));
	
	switch (type)
	{
	case EInteractionType::Default:
		return true;
	case EInteractionType::Storage_Check:
		return true;
	case EInteractionType::Storage_Take:
		Take(20);
		return true;
	case EInteractionType::Storage_Refill:
		Deposit(20);
		return true;
	}

	return false;
}

bool ABaseStation::IsInteractionComplete_Implementation()
{
	UE_LOG(LogTemp, Warning, TEXT("Base station is interaction complete called"));

	return true;
}

void ABaseStation::GatherWorldFacts_Implementation(TArray<FWorldFact>& OutFacts)
{
	FWorldFact Fact;
	Fact.Key = WorldFactKeyName;
	Fact.BaseType = EWorldBaseType::Station;
	Fact.InfoType = FactType;
	Fact.BoolValue = bIsStationActive;
	Fact.IntValue = ResourceAmount;
	Fact.Location = GetActorLocation();
	Fact.Source = TWeakObjectPtr<AActor>(this);
	Fact.SingleOwner = false;
	Fact.IsAlreadyTaken = false;

	OutFacts.Add(Fact);
}

void ABaseStation::ActivateStation()
{
	bIsStationActive = true;
}

void ABaseStation::DeactivateStation()
{
	bIsStationActive = false;
}

void ABaseStation::Take(int Amount)
{
	if (!CanTake(Amount)) return;

	ResourceAmount -= Amount;

	ResourceAmount = FMath::Clamp(ResourceAmount, 0, MaxResourceAmount);
}

void ABaseStation::Deposit(int Amount)
{
	if (!CanDeposit(Amount)) return;

	ResourceAmount += Amount;

	ResourceAmount = FMath::Clamp(ResourceAmount, 0, MaxResourceAmount);
}

bool ABaseStation::CanTake(int Amount) const
{
	return ResourceAmount > Amount;
}

bool ABaseStation::CanDeposit(int Amount) const
{
	return ResourceAmount + Amount <= MaxResourceAmount;
}

void ABaseStation::BeginPlay()
{
	Super::BeginPlay();
	TextRenderer->SetText(FText::FromString(DisplayName));
	TextRenderer->SetRelativeLocation(FVector(0, 0, 100));
	TextRenderer->SetWorldSize(40);

	if (UGameInstance* GI = GetGameInstance())
	{
		if (UWorldFactRegistry* Registry = GI->GetSubsystem<UWorldFactRegistry>())
		{
			Registry->Register(this);
		}
	}
}

void ABaseStation::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	UpdateTextRotation();
}

void ABaseStation::UpdateTextRotation()
{
	if (!TextRenderer) return;

	APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0);
	if (!PC) return;

	FVector CameraLocation = PC->PlayerCameraManager->GetCameraLocation();
	
	FVector TextLocation = TextRenderer->GetComponentLocation();
	
	TextRenderer->SetWorldRotation((CameraLocation - TextLocation).Rotation());
}

