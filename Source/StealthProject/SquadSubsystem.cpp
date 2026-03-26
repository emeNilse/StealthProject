// Fill out your copyright notice in the Description page of Project Settings.


#include "SquadSubsystem.h"
#include "SquadComponent.h"
#include "UObject/ConstructorHelpers.h"

USquadSubsystem::USquadSubsystem()
{
    static ConstructorHelpers::FObjectFinder<USquadConfigData> ConfigObj(TEXT("/Game/ThirdPerson/AI/Controller/DA_SquadConfig.DA_SquadConfig"));

    if (ConfigObj.Succeeded())
    {
        SquadConfig = ConfigObj.Object;
    }
}

void USquadSubsystem::Register(USquadComponent* squadActor)
{
    if (squadActor)
    {
        SquadAgents.Add(squadActor);
    }

    if (!bClusterBuildScheduled)
    {
        bClusterBuildScheduled = true;

        FTimerHandle timerHandle;
        GetWorld()->GetTimerManager().SetTimer(timerHandle, this, &USquadSubsystem::BuildClusters, 0.1f, false);
    }
}

void USquadSubsystem::BuildClusters()
{
    while (!SquadAgents.IsEmpty())
    {
        TWeakObjectPtr<USquadComponent> agent = SquadAgents[0];
        SquadAgents.RemoveAt(0);

        Cluster newCluster;
        newCluster.Members.Add(agent);
        FVector location;

        if (!agent.IsValid()) continue;

        location = agent->CachedOwner->GetActorLocation();
        TArray<TWeakObjectPtr<USquadComponent>> removeList;

        for (const TWeakObjectPtr<USquadComponent>& other : SquadAgents)
        {
            if (!other.IsValid()) continue;
            
            AActor* otherActor = other.Get()->GetOwner();

            if (!otherActor) continue;
            
            FVector otherLocation = otherActor->GetActorLocation();

            if (FVector::DistSquared(location, otherLocation) < pow(ClusterRadius, 2))
            {
                newCluster.Members.Add(other);
                
                removeList.Add(other);
            }
        }

        Clusters.Add(newCluster);
        for (TWeakObjectPtr<USquadComponent> other : removeList)
        {
            SquadAgents.Remove(other);
        }
    }

    for (Cluster clust : Clusters)
    {
        if (clust.Members.Num() >= 3)
        {
            clust.Center = CalculateAverageCenter(clust);
            CreateSquadManager(clust);
        }
    }
}

FVector USquadSubsystem::CalculateAverageCenter(Cluster cluster)
{
    FVector sumOfVectors = FVector::ZeroVector;
    FVector averageLocation;

    for (const TWeakObjectPtr<USquadComponent>& member : cluster.Members)
    {
        sumOfVectors += member->CachedOwner->GetActorLocation();
    }

    averageLocation = sumOfVectors / cluster.Members.Num();
    
    return averageLocation;
}

void USquadSubsystem::CreateSquadManager(Cluster cluster)
{
    ASquadManager* squad = GetWorld()->SpawnActor<ASquadManager>(ASquadManager::StaticClass(), cluster.Center, FRotator::ZeroRotator);

    squad->ConfigInitialize(SquadConfig);

    if (squad)
    {
        squad->Initialize(cluster.Members);
        TWeakObjectPtr<ASquadManager> weakSquad = squad;
        ActiveSquads.Add(weakSquad);
    }
}

void USquadSubsystem::RemoveSquad(TWeakObjectPtr<ASquadManager> deadSquad)
{
    ActiveSquads.Remove(deadSquad);
}
