
#include "BaseStation.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"
#include "Camera/PlayerCameraManager.h"

ABaseStation::ABaseStation()
{
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	/*TextRenderer = CreateDefaultSubobject<UTextRenderComponent>(TEXT("TextRenderer"));
	TextRenderer->SetupAttachment(RootComponent);*/

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
	Fact.Key = KeyName;
	Fact.BoolValue = bIsStationActive;
	Fact.IntValue = ResourceAmount;
	Fact.FloatValue = ResourceFloat;
	Fact.Location = GetActorLocation();
	Fact.Source = TWeakObjectPtr<AActor>(this);

	OutFacts.Add(Fact);
}

void ABaseStation::Take(int32 Amount)
{
	if (!CanTake(Amount)) return;

	ResourceAmount -= Amount;

	ResourceAmount = FMath::Clamp(ResourceAmount, 0, MaxResourceAmount);
}

void ABaseStation::Deposit(int32 Amount)
{
	if (!CanDeposit(Amount)) return;

	ResourceAmount += Amount;

	ResourceAmount = FMath::Clamp(ResourceAmount, 0, MaxResourceAmount);
}

bool ABaseStation::CanTake(int32 Amount) const
{
	return ResourceAmount > Amount;
}

bool ABaseStation::CanDeposit(int32 Amount) const
{
	return ResourceAmount + Amount <= MaxResourceAmount;
}

void ABaseStation::BeginPlay()
{
	Super::BeginPlay();
	TextRenderer->SetText(FText::FromString(DisplayName));
	TextRenderer->SetRelativeLocation(FVector(0, 0, 100));
	TextRenderer->SetWorldSize(40);
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

