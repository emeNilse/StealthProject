// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Info.h"
#include "AgentfactProvider.h"
#include "NPC.h"
#include "SquadManager.generated.h"

UENUM(BlueprintType)
enum class ESquadRole : uint8
{
	Assualt,
	Support,
	Skirmisher
};

UENUM(BlueprintType)
enum class ESquadState : uint8
{
	Neutral,
	Alert,
	Combat
};

UCLASS()
class STEALTHPROJECT_API ASquadManager : public AInfo
{
	GENERATED_BODY()

public:
	ASquadManager();

	float Radius;

	UFUNCTION()
	void FindMembers();

	UFUNCTION()
	void RoleAssignemnt();

protected:
	virtual void BeginPlay() override;

private:
	FVector ManagerLocation;

	TArray<AActor*> MySquad;
	
};
