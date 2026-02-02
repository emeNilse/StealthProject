// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/TextRenderComponent.h"
#include "Interactable.h"
#include "BaseStation.generated.h"

UCLASS()
class STEALTHPROJECT_API ABaseStation : public AActor, public IInteractable
{
	GENERATED_BODY()
	
public:	

	ABaseStation();

protected:

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere)
	UTextRenderComponent* TextRenderer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString DisplayName;

private:
	void UpdateTextRotation();
};
