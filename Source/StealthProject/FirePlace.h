// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseStation.h"
#include "GoapWorldStateComponent.h"
#include "WorldFactRegistry.h"
#include "FirePlace.generated.h"

UCLASS()
class STEALTHPROJECT_API AFirePlace : public ABaseStation
{
	GENERATED_BODY()
	
public:	
	AFirePlace();

	virtual bool Interact_Implementation(AActor* interactor, EInteractionType type) override;

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
