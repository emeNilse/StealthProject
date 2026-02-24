
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

//The AI Controller is responsible for movement and is the part that has access to the Blackboard component.
//I used a Behaviour Tree to get an understanding of how to access the Blackboard and to test the GOAP in its early stages.
//You will likely see remnants of my Behaviour Tree code lying around, because I want to have it available for debugging purposes.
//I use Unreal's AI perception system for sight and hearing, of which, the parameters are configured in the SetupPerceptionSystem().
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
