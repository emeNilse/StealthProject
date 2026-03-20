// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "SquadManager.h"
#include "SquadComponent.h"
#include "SquadSubsystem.generated.h"



struct Cluster
{
	TArray<TWeakObjectPtr<USquadComponent>> Members;
	FVector Ceneter;
};

UCLASS()
class STEALTHPROJECT_API USquadSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	TArray<TWeakObjectPtr<USquadComponent>> SquadProviders;

	void Register(USquadComponent* squadActor);

	void BuildClusters(TArray<TWeakObjectPtr<USquadComponent>> agents, float radius);

	FVector CalculateAverageCenter(Cluster cluster);

	void CreateSquadManager(Cluster cluster);

private:
	TArray<Cluster> Clusters;
};
