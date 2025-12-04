// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AgentBeliefs.h"

//class AGoapAgent;
class USensor;
class UGoapComponent;
/**
 * 
 */
class STEALTHPROJECT_API BeliefFactory
{
private:

	UGoapComponent* Component;
	TMap<FString, TSharedPtr<AgentBeliefs>>& Beliefs;


public:
	BeliefFactory(UGoapComponent* component, TMap<FString, TSharedPtr<AgentBeliefs>>& beliefs);
	~BeliefFactory();

	void AddBelief(FString key, TFunction<bool()> condition);

	//void AddBelief(TSharedPtr<AgentBeliefs>& belief, TFunction<bool()> condition);

	void AddSensorBelief(FString key, USensor* sensor);

	void AddLocationBelief(FString key, float distance, FVector& locationCondition);

	//void AddLocationBelief(AGoapAgent& agent, FString key, float distance, const AActor* locationOfActor);

	bool InRangeOf(FVector position, float range);
};
