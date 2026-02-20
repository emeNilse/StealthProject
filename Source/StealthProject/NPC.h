// Fill out your copyright notice in the Description page of Project Settings.

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
#include "EdGraphSchema_K2.h"
#include "Perception/AISenseConfig_Sight.h"
#include "ProceduralMeshComponent.h"
#include "NPC.generated.h"

UENUM(BlueprintType)
enum class ENPCState : uint8
{
	Calm,
	Investigative,
	Alert,
	Engaged
};

UCLASS()
class STEALTHPROJECT_API ANPC : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ANPC();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION()
	virtual void RayCast();

	UBehaviorTree* GetBehaviorTree() const;

	UBlackboardData* GetBlackBoardData() const;

	APatrolPath* GetPatrolPath();

	void SetPatrolPath(APatrolPath* inPath);

	void UpdateStats();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<FName, float> Stats;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UProceduralMeshComponent* VisionMesh;

	UPROPERTY(EditDefaultsOnly)
	UMaterialInterface* VisionMaterial;

	/*UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* ConeMesh;*/

	UFUNCTION(BlueprintCallable)
	void SetDestination(FVector TargetDestination);

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
	void BeginAlert();

	UFUNCTION(BlueprintCallable)
	void BeginInvestigative();

	UFUNCTION(BlueprintCallable)
	void OnNPCStateChange();

	void InitializeVisionCone();

	void GenerateVisualCone(float Radius, float HalfAngleDegrees, int32 NumSegments);

	void Generate3DVisual(float Height, float Radius, int32 Sides);

	virtual void PossessedBy(AController* NewController) override;

	bool bRecharging = false;

	FVector GlobalDestination;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AI")
	UBlackboardData* BlackboardAsset;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
	UGoapWorldStateComponent* WorldState;

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI", meta=(AllowPrivateAccess = "true"))
	UBehaviorTree* Tree;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI", meta = (AllowPrivateAccess = "true"))
	APatrolPath* PatrolPath;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI", meta = (AllowPrivateAccess = "true"))
	UAISenseConfig_Sight* SightConfiguration;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI", meta = (AllowPrivateAccess = "true"))
	AAI_Controller* MyAIController;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI", meta = (AllowPrivateAccess = "true"))
	ENPCState NPCState = ENPCState::Calm;

	UMaterialInstanceDynamic* DynMaterial;

	UMaterialInstanceDynamic* DynMaterialForScanner;

	UStaticMeshComponent* ConeMesh;

	FTimerHandle AlertTimerhandle;

	FTimerHandle InvestigativeTimerhandle;

	bool PlayerSpotted = false;

	float StatTimerInterval;
	float StatTimerRemaining;

};
