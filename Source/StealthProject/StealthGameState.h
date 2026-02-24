
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "StealthGameState.generated.h"

//I made this game state so that if one AI sees the player and loses sight of them, then he can tell
//all of his AI friends to be on alert (for 30 seconds, see NPC).

UCLASS()
class STEALTHPROJECT_API AStealthGameState : public AGameStateBase
{
	GENERATED_BODY()
public:

	AStealthGameState();

	virtual void BeginPlay() override;

	void SetGlobalAlert();
};
