
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
		if (It->GetNPCState() == ENPCState::Calm || It->GetNPCState() == ENPCState::Investigative)
		{
			It->SetNPCState(ENPCState::Alert);
		}
	}
}
