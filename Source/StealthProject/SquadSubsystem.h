// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "SquadManager.h"
#include "SquadSubsystem.generated.h"

class USquadComponent;

struct Cluster
{
	TArray<TWeakObjectPtr<USquadComponent>> Members;
	FVector Center;
};

UCLASS()
class STEALTHPROJECT_API USquadSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()
	
public:
	USquadConfigData* SquadConfig;
	
	TArray<TWeakObjectPtr<USquadComponent>> SquadAgents;

	USquadSubsystem();

	void Register(USquadComponent* squadActor);

	void BuildClusters();

	FVector CalculateAverageCenter(Cluster cluster);

	void CreateSquadManager(Cluster cluster);

	void RemoveSquad(TWeakObjectPtr<ASquadManager> deadSquad);

private:
	TArray<Cluster> Clusters;

	TArray<TWeakObjectPtr<ASquadManager>> ActiveSquads;

	bool bClusterBuildScheduled = false;

	float ClusterRadius = 5000;
};
