
#pragma once

#include "CoreMinimal.h"
#include "PatrolPath.h"
#include "AI_Controller.h"
#include "GameFramework/Character.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"
#include "DrawDebugHelpers.h"
#include "BehaviorTree/BehaviorTree.h"
#include "GoapWorldStateComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "ProceduralMeshComponent.h"
#include "AgentfactProvider.h"
#include "NPC.generated.h"

//The NPC class primarily handles AI state and stats. And is the link between AI Controller and Blackboard.


//States for the AI, used as requirements for Actions and visuals
//Calm -> Run daily operations
//Investigative -> investigate anomaly (noise)
//Alert -> search the area, type of search depends on Suspicious meter
//Engaged -> Chase the player

//UENUM(BlueprintType)
//enum class ENPCState : uint8
//{
//	Calm,
//	Investigative,
//	Alert,
//	Engaged
//};

UCLASS()
class STEALTHPROJECT_API ANPC : public ACharacter, public IAgentFactProvider
{
	GENERATED_BODY()

public:
	ANPC();

	UBehaviorTree* GetBehaviorTree() const;

	UBlackboardData* GetBlackBoardData() const;

	APatrolPath* GetPatrolPath();

	bool bRecharging = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<FName, float> Stats;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UProceduralMeshComponent* VisionMesh;

	UPROPERTY(EditDefaultsOnly)
	UMaterialInterface* VisionMaterial;

	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION()
	virtual void RayCast();

	void SetPatrolPath(APatrolPath* inPath);

	UFUNCTION(BlueprintCallable)
	float GetStat(FName StatName) const;

	UFUNCTION(BlueprintCallable)
	void SetStat(FName StatName, float NewValue);

	UFUNCTION(BlueprintCallable)
	void ModifyStat(FName StatName, float Delta);

	UFUNCTION(BlueprintCallable)
	ENPCState GetNPCState() const { return NPCState; }

	UFUNCTION(BlueprintCallable)
	void SetNPCState(ENPCState NewState);

	UFUNCTION(BlueprintCallable)
	void ReturnToCalm();

	UFUNCTION(BlueprintCallable)
	void ReturnToAlert();

	UFUNCTION(BlueprintCallable)
	void BeginAlert();

	UFUNCTION(BlueprintCallable)
	void BeginInvestigative();

	UFUNCTION(BlueprintCallable)
	void OnNPCStateChange();

	virtual void PossessedBy(AController* NewController) override;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AI")
	UBlackboardData* BlackboardAsset;

	//GOAP world state properties, this is for the AI to keep track of world information.
	//e.g. some station (will eventually) have information that the AI should know about.
	//The AI will use this in it's planner to determine where to go for resources and other items.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
	UGoapWorldStateComponent* WorldState;

private:
	//My old Behaviour Tree, as stated in other scripts, I'm keeping it around for debugging purposes
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI", meta=(AllowPrivateAccess = "true"))
	UBehaviorTree* Tree;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI", meta = (AllowPrivateAccess = "true"))
	APatrolPath* PatrolPath;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI", meta = (AllowPrivateAccess = "true"))
	AAI_Controller* MyAIController;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI", meta = (AllowPrivateAccess = "true"))
	ENPCState NPCState = ENPCState::Calm;

	ENPCState LastNPCState = ENPCState::Calm;

	UMaterialInstanceDynamic* DynMaterial;

	UMaterialInstanceDynamic* DynMaterialForScanner;

	UStaticMeshComponent* ConeMesh;

	FTimerHandle AlertTimerhandle;

	FTimerHandle InvestigativeTimerhandle;

	bool PlayerSpotted = false;

	float StatTimerInterval;

	float StatTimerRemaining;

	void UpdateStats();

	void InitializeVisionCone();

	void GenerateVisualCone(float Radius, float HalfAngleDegrees, int32 NumSegments);

	void Generate3DVisual(float Height, float Radius, int32 Sides);
};
