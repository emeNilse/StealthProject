// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "WorldFactProvider.h"
#include "WorldFactRegistry.generated.h"

/**
 * 
 */
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

	const TArray<TWeakObjectPtr<AActor>>& GetProviders() const
	{
		return Providers;
	}
};
