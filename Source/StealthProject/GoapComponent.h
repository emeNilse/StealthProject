// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GoapBelief.h"
#include "GoapBeliefSet.h"
#include "BooleanBelief.h"
#include "BlackboardBoolBelief.h"
#include "LocationBelief.h"
#include "MovementStatusBelief.h"
#include "DistanceToActorBelief.h"
#include "GoapAction.h"
#include "GoapActionsSet.h"
#include "GoapGoal.h"
#include "GoapGoalObject.h"
#include "CountdownTimer.h"
#include "ActionStackComponent.h"
#include "ActionPlan.h"
#include "GoapPlanner.h"
#include "AgentBeliefs.h"
#include "GoapActionStrategyBase.h"
#include "IdleStrategy.h"
#include "MoveStrategy.h"
#include "RechargeStrategy.h"
#include "PatrolStrategy.h"
#include "AttackStrategy.h"
#include "ChasePlayerStrategy.h"
#include "GoapPlannerInterface.h"
#include "GoapFactorySubsystem.h"
#include "GoapComponent.generated.h"

class AAI_Controller;
class BeliefFactory;
class ANPC;

struct NPCState
{
	float Stamina;
	float OilAmountAtStation;
	int RefinedOreAtRefinery;
	
	bool bCanSeePlayer;
};


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class STEALTHPROJECT_API UGoapComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGoapComponent();

	AAI_Controller* AI;
	ANPC* NPC;
	UBlackboardComponent* AI_BlackBoard;
	UActionStackComponent* ActionStackComponent;

	TSharedPtr<GoapAction> CurrentAction;

	TSet<TSharedPtr<GoapAction>> Actions;
	
	TSharedPtr<GoapGoal> CurrentGoal;

	TSet<TSharedPtr<GoapGoal>> Goals;

	TSharedPtr<ActionPlan> TheActionPlan;
	TMap<FString, TSharedPtr<AgentBeliefs>> Beliefs;

	TSharedPtr<IGoapPlannerInterface> GoapPlanner;
	UGoapFactorySubsystem* GoapFactory;
	TUniquePtr<BeliefFactory> Factory;

	NPCState LastNPCState;

	UPROPERTY(EditAnywhere, Instanced, BlueprintReadWrite)
	TArray<UGoapBelief*> uBeliefs;

	UPROPERTY(EditAnywhere, Instanced, BlueprintReadWrite)
	TArray<UGoapActionsSet*> uActions;

	UPROPERTY(EditAnywhere, Instanced, BlueprintReadWrite)
	TArray<UGoapGoalObject*> uGoals;


	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FVector Destination;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	AActor* Target;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	AActor* RechargeStationActor;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector RechargeStation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector SupplyStation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector Mine;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector OilWell;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector Refinery;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector SupplyShipmentStation;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "TEXT")
	FString CurrentActionText;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "TEXT")
	FString CurrentGoalText;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void MakeAPlanForActionStack();

	void HandlePlanFailed();

	void HandlePlanFinished();

	void SetupBeliefs();

	void SetupAction();

	void SetupGoals();

	void CalculatePlan();

	void UpdateNPCState();

	bool HasNPCStateChanged();

	UFUNCTION(BlueprintCallable)
	UGoapActionStrategyBase* GetCurrentStrategy() const { return CurrentAction->Strategy; }

	UFUNCTION(BlueprintCallable)
	FString GetCurrentGoal() const { return CurrentGoal->Name; }

private:

	TSharedPtr<GoapGoal> LastGoal;
};
