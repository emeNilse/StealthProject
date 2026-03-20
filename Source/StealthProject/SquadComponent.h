// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SquadManager.h"
#include "SquadComponent.generated.h"



UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class STEALTHPROJECT_API USquadComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	USquadComponent();

	ASquadManager* SquadID;

	ESquadRole SquadRole;

	ESquadState SquadState;

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
