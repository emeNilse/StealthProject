// Fill out your copyright notice in the Description page of Project Settings.


#include "StealthGameState.h"
#include "EngineUtils.h"
#include "NPC.h"

AStealthGameState::AStealthGameState()
{
}

void AStealthGameState::BeginPlay()
{
	Super::BeginPlay();
	UE_LOG(LogTemp, Warning, TEXT("StealthGameState Active"));
}

void AStealthGameState::SetGlobalAlert()
{
	for (TActorIterator<ANPC> It(GetWorld()); It; ++It)
	{
		It->SetNPCState(ENPCState::Alert);
	}
}
