
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "WorldFactProvider.h"
#include "GoapWorldStateComponent.generated.h"

//Goap World State Component keeps track of any world facts that are provided from stations in the game world.
//Any station that has world facts to share gets registered, and upon call asks for a refresh of those facts.
//Added to my AIs, but currently not in direct use.

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
