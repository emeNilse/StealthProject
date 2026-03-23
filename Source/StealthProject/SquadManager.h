// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Info.h"
#include "AgentfactProvider.h"
#include "NPC.h"
#include "SquadManager.generated.h"


class USquadComponent;

UENUM(BlueprintType)
enum class ESquadRole : uint8
{
	Default,
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

	void Initialize(TArray<TWeakObjectPtr<USquadComponent>> members);

	ESquadState GetSquadState() const { return SquadState; }

	UFUNCTION()
	void FindMembers();

	UFUNCTION()
	void RoleAssignemnt();

	UFUNCTION()
	void NotifyMemberDied(USquadComponent* deadMember);

	void ChangeState(ESquadState newState);

	void OnStateChange();

protected:
	virtual void BeginPlay() override;

private:
	FVector ManagerLocation;

	TArray<USquadComponent*> MySquad;

	ESquadState SquadState;

	AActor* CurrentTarget;

	int AssualtRolesAvailable = 1;
	int SkirmisherRolesAvailable = 2;
	
};
