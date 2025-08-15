// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "AI_Controller.generated.h"

/**
 * 
 */
UCLASS()
class STEALTHPROJECT_API AAI_Controller : public AAIController
{
	GENERATED_BODY()

public:

	explicit AAI_Controller(FObjectInitializer const& ObjectInitializer);
	

protected: 

	virtual void OnPossess(APawn* InPawn) override;
};
