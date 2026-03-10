
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
	UE_LOG(LogTemp, Warning, TEXT("Base station interaction called"))
	
	return true;
}

void ABaseStation::GatherWorldFacts_Implementation(TArray<FWorldFact>& OutFacts)
{
	FWorldFact Fact;
	Fact.Key = "IsStation";
	Fact.Type = EWorldFactType::Bool;
	Fact.IntValue = true;
	Fact.Location = GetActorLocation();
	Fact.Source = TWeakObjectPtr<AActor>(this);

	OutFacts.Add(Fact);
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

