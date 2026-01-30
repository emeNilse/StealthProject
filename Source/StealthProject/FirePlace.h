// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseStation.h"
#include "WorldFactProvider.h"
#include "WorldStateSubsystem.h"
#include "FirePlace.generated.h"

UCLASS()
class STEALTHPROJECT_API AFirePlace : public ABaseStation, public IWorldFactProvider
{
	GENERATED_BODY()
	
public:	
	AFirePlace();

	virtual void GatherWorldFacts_Implementation(TArray<FWorldFact>& OutFacts) override;

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	void IgniteFire();

private:

	bool bFireActive;

	float TimeActive;

};
