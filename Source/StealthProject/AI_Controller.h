// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AIPerceptionTypes.h"
#include "AI_Controller.generated.h"

class UGoapComponent;
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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UBlackboardComponent* BlackboardComponent;

private:
	class UAISenseConfig_Sight* SightConfig;

	UGoapComponent* Goap;

	void SetupPerceptionSystem();

	UFUNCTION()
	void OnTargetDetected(AActor* Actor, FAIStimulus const Stimulus);
};
