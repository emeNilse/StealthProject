// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PatrolPath.h"
#include "GameFramework/Character.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"
#include "DrawDebugHelpers.h"
#include "BehaviorTree/BehaviorTree.h"
#include "EdGraphSchema_K2.h"

#include "NPC.generated.h"

//must this inherit from actionstack???
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

	UFUNCTION(BlueprintCallable)
	void SetDestination(FVector TargetDestination);

	UFUNCTION(BlueprintCallable)
	float GetStat(FName StatName) const;

	UFUNCTION(BlueprintCallable)
	void SetStat(FName StatName, float NewValue);

	UFUNCTION(BlueprintCallable)
	void ModifyStat(FName StatName, float Delta);

	bool bRecharging = false;

	float Stamina;

	FVector GlobalDestination;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AI")
	UBlackboardData* BlackboardAsset;

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI", meta=(AllowPrivateAccess = "true"))
	UBehaviorTree* Tree;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI", meta = (AllowPrivateAccess = "true"))
	APatrolPath* PatrolPath;


	float StatTimerInterval;
	float StatTimerRemaining;

};
