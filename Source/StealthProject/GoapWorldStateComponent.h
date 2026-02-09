// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
//#include "Subsystems/GameInstanceSubsystem.h"
#include "Components/ActorComponent.h"
#include "WorldFactProvider.h"
#include "GoapWorldStateComponent.generated.h"

/**
 * 
 */
UCLASS()
class STEALTHPROJECT_API UGoapWorldStateComponent : public UActorComponent
{
	GENERATED_BODY()
	
public:
	TArray<TWeakObjectPtr<AActor>> FactProviders;
	TArray<FWorldFact> CachedFacts;

	void RegisterProvider(AActor* actor)
	{
		if (actor->Implements<UWorldFactProvider>())
		{
			FactProviders.Add(actor);
		}
	}

	void RebuildWorldFacts()
	{
		CachedFacts.Reset();

		for (auto provider : FactProviders)
		{
			if (!provider.IsValid()) continue;

			IWorldFactProvider::Execute_GatherWorldFacts(provider.Get(), CachedFacts);
		}
	}

	const TArray<FWorldFact>& GetFacts() const
	{
		return CachedFacts;
	}
};
