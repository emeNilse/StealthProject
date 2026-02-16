// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseStation.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"
#include "Camera/PlayerCameraManager.h"

// Sets default values
ABaseStation::ABaseStation()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	TextRenderer = CreateDefaultSubobject<UTextRenderComponent>(TEXT("TextRenderer"));
	TextRenderer->SetupAttachment(RootComponent);

	DisplayName = TEXT("Station name");
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

// Called when the game starts or when spawned
void ABaseStation::BeginPlay()
{
	Super::BeginPlay();
	TextRenderer->SetText(FText::FromString(DisplayName));
	TextRenderer->SetRelativeLocation(FVector(0, 0, 100));
	TextRenderer->SetWorldSize(40);
}

// Called every frame
void ABaseStation::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	UpdateTextRotation();
}

void ABaseStation::UpdateTextRotation()
{
	if (!TextRenderer) return;

	APlayerController* pc = UGameplayStatics::GetPlayerController(this, 0);
	if (!pc) return;

	FVector CameraLocation = pc->PlayerCameraManager->GetCameraLocation();
	
	FVector TextLocation = TextRenderer->GetComponentLocation();
	
	TextRenderer->SetWorldRotation((CameraLocation - TextLocation).Rotation());
}

