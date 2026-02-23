// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AIPerceptionTypes.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AISenseConfig_Hearing.h"
#include "NavigationSystem.h"
#include "Navigation/PathFollowingComponent.h"
#include "NavigationPath.h"
#include "AI_Controller.generated.h"

class UGoapComponent;
class ANPC;
/**
 * 
 */
UCLASS()
class STEALTHPROJECT_API AAI_Controller : public AAIController
{
	GENERATED_BODY()

public:

	explicit AAI_Controller(FObjectInitializer const& ObjectInitializer);
	
	UAISenseConfig_Sight* GetSightConfig() const { return SightConfig; }

protected: 

	virtual void OnPossess(APawn* InPawn) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UBlackboardComponent* BlackboardComponent;

private:
	class UAISenseConfig_Sight* SightConfig;

	class UAISenseConfig_Hearing* HearingConfig;

	UGoapComponent* Goap;

	ANPC* MyNPC;

	UNavigationSystemV1* NavSys;

	void SetupPerceptionSystem();

	UFUNCTION()
	void OnPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);

	UFUNCTION()
	void OnTargetDetected(AActor* Actor, FAIStimulus const Stimulus);

	UFUNCTION()
	void OnNoiseHeard(AActor* Actor, FAIStimulus const Stimulus);
};
