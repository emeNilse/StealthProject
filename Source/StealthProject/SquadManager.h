// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Info.h"
#include "AgentfactProvider.h"
#include "NPC.h"
#include "SquadManager.generated.h"

/**
 * 
 */
UCLASS()
class STEALTHPROJECT_API ASquadManager : public AInfo
{
	GENERATED_BODY()

public:
	ASquadManager();

	UPROPERTY()
	TArray<AActor*> MySquad;

	UPROPERTY()
	float Radius;


	UFUNCTION()
	void FindMembers();

	UFUNCTION()
	void RoleAssignemnt();

protected:
	virtual void BeginPlay() override;

private:

	
};
