// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GoapAction.h"
#include "GoapGoal.h"
#include "ActionPlan.h"
#include "AI_Controller.h"
#include "AgentBeliefs.h"
#include "BeliefFactory.h"
#include "GoapPlannerInterface.h"
#include "GoapFactorySubsystem.h"
#include "GoapComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class STEALTHPROJECT_API UGoapComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGoapComponent();

	AAI_Controller* AI;

	GoapAction* CurrentAction;
	TSet<GoapAction*> Actions;

	GoapGoal* CurrentGoal;
	TSet<GoapGoal*> Goals;

	ActionPlan* ActionPlan;
	TMap<FString, TSharedPtr<AgentBeliefs>> Beliefs;

	TSharedPtr<IGoapPlannerInterface> GoapPlanner;
	UGoapFactorySubsystem* GoapFactory;

	UPROPERTY()
	FVector Destination;
	UPROPERTY()
	AActor* Target;
	UPROPERTY()
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
	float Stamina;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void SetupTimers();

	void SetupBeliefs();

	void SetupAction();

	void SetupGoals();

private:

	GoapGoal* LastGoal;
};
