// Fill out your copyright notice in the Description page of Project Settings.


#include "SquadSubsystem.h"

void USquadSubsystem::Register(USquadComponent* squadActor)
{
    if (squadActor)
    {
        SquadProviders.Add(squadActor);
    }
}

void USquadSubsystem::BuildClusters(TArray<TWeakObjectPtr<USquadComponent>> agents, float radius)
{
    while (!agents.IsEmpty())
    {
        TWeakObjectPtr<USquadComponent> agent = agents[0];
        agents.RemoveAt(0);

        Cluster newCluster;
        newCluster.Members.Add(agent);
        FVector location;

        if (agent.IsValid())
        {
            AActor* actor = agent.Get()->GetOwner();
            if (actor)
            {
                location = actor->GetActorLocation();
            }
        }

        for (TWeakObjectPtr<USquadComponent> other : agents)
        {
            AActor* otherActor = other.Get()->GetOwner();
            if (otherActor)
            {
                FVector otherLocation = otherActor->GetActorLocation();

                if (FVector::DistSquared(location, otherLocation) < pow(radius, 2))
                {
                    newCluster.Members.Add(other);
                    agents.Remove(other);
                }
            } 
        }

        Clusters.Add(newCluster);
    }

    for (Cluster clust : Clusters)
    {
        if (clust.Members.Num() >= 3)
        {
            clust.Ceneter = CalculateAverageCenter(clust);
            CreateSquadManager(clust);
        }
    }
}

FVector USquadSubsystem::CalculateAverageCenter(Cluster cluster)
{
    return FVector();
}

void USquadSubsystem::CreateSquadManager(Cluster cluster)
{
    
}
