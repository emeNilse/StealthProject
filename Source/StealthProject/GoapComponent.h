// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GoapAction.h"
#include "GoapGoal.h"
#include "CountdownTimer.h"
#include "ActionPlan.h"
#include "GoapPlanner.h"
#include "AgentBeliefs.h"
#include "IdleStrategy.h"
#include "MoveStrategy.h"
#include "PatrolStrategy.h"
#include "AttackStrategy.h"
#include "GoapPlannerInterface.h"
#include "GoapFactorySubsystem.h"
#include "GoapComponent.generated.h"

class AAI_Controller;
class BeliefFactory;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class STEALTHPROJECT_API UGoapComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGoapComponent();

	AAI_Controller* AI;
	UBlackboardComponent* AI_BlackBoard;

	TUniquePtr<CountdownTimer> StatTimer;
	bool bShouldReplan;

	TSharedPtr<GoapAction> CurrentAction;
	TSet<TSharedPtr<GoapAction>> Actions;

	TSharedPtr<GoapGoal> CurrentGoal;
	TSet<TSharedPtr<GoapGoal>> Goals;

	TSharedPtr<ActionPlan> TheActionPlan;
	TMap<FString, TSharedPtr<AgentBeliefs>> Beliefs;

	TSharedPtr<IGoapPlannerInterface> GoapPlanner;
	UGoapFactorySubsystem* GoapFactory;
	TUniquePtr<BeliefFactory> Factory;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FVector Destination;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	AActor* Target;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector RechargeStation;
	UPROPERTY()
	FVector SupplyStation;
	UPROPERTY()
	FVector Mine;
	UPROPERTY()
	FVector OilWell;
	UPROPERTY()
	FVector Refinery;
	UPROPERTY()
	FVector SupplyShipmentStation;
	UPROPERTY()
	float Health;
	UPROPERTY()
	float Stamina = 100;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void SetupBeliefs();

	void SetupAction();

	void SetupGoals();

	void SetupTimers();

	void UpdateStats();

	void CalculatePlan();

	void RequestReplan();

private:

	TSharedPtr<GoapGoal> LastGoal;
};
