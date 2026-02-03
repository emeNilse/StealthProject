// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/TextRenderComponent.h"
#include "Interactable.h"
#include "WorldFactProvider.h"
#include "WorldStateSubsystem.h"
#include "BaseStation.generated.h"

UCLASS()
class STEALTHPROJECT_API ABaseStation : public AActor, public IInteractable, public IWorldFactProvider
{
	GENERATED_BODY()
	
public:	

	ABaseStation();

	virtual void GatherWorldFacts_Implementation(TArray<FWorldFact>& OutFacts) override;

protected:

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere)
	UTextRenderComponent* TextRenderer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString DisplayName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName StationTag;

private:
	void UpdateTextRotation();
};
