
#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "WorldFactProvider.h"
#include "WorldFactRegistry.generated.h"

//Subsystem that stores any Actor that implements the World Fact Provider.

UCLASS()
class STEALTHPROJECT_API UWorldFactRegistry : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	TArray<TWeakObjectPtr<AActor>> Providers;

	void Register(AActor* Actor)
	{
		if (Actor && Actor->Implements<UWorldFactProvider>())
		{
			Providers.AddUnique(Actor);
		}
	}

	const TArray<TWeakObjectPtr<AActor>>& GetProviders() const { return Providers; }
};
