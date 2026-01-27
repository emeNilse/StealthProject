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

	TextRenderer = CreateDefaultSubobject<UTextRenderComponent>(TEXT("TextRenderer"));
	RootComponent = TextRenderer;

	DisplayName = TEXT("Station name");
}

// Called when the game starts or when spawned
void ABaseStation::BeginPlay()
{
	Super::BeginPlay();
	TextRenderer->SetText(FText::FromString(DisplayName));
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

	FRotator LookAtRotation = (CameraLocation - TextLocation).Rotation();
	TextRenderer->SetWorldRotation(LookAtRotation);
}

