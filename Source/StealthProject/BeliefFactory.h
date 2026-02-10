// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AgentBeliefs.h"

class USensor;
class UGoapComponent;
/**
 * 
 */
class STEALTHPROJECT_API BeliefFactory
{
private:

	UGoapComponent* Component;
	//BeliefRegistry& Registry;

	AgentBeliefs::BeliefRegistry& Registry;
	//TMap<FString, TSharedPtr<AgentBeliefs>>& Beliefs;


public:
	BeliefFactory(UGoapComponent* component, AgentBeliefs::BeliefRegistry& registry);
	//TMap<FString, TSharedPtr<AgentBeliefs>>& beliefs
	~BeliefFactory();

	void AddBelief(FString key, TFunction<bool()> condition);


	/*void AddSensorBelief(FString key, USensor* sensor);

	void AddLocationBelief(FString key, float distance, FVector& locationCondition);

	bool InRangeOf(FVector position, float range);*/
};
