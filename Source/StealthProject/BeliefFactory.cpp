// Fill out your copyright notice in the Description page of Project Settings.


#include "BeliefFactory.h"
#include "GoapComponent.h"


BeliefFactory::BeliefFactory(UGoapComponent* component, TMap<FString, TSharedPtr<AgentBeliefs>>& beliefs) : Component(component), Beliefs(beliefs)
{}

BeliefFactory::~BeliefFactory()
{
}

void BeliefFactory::AddBelief(FString key, TFunction<bool()> condition)
{
	Beliefs.Add(key, AgentBeliefs::Builder(key).WithCondition(condition).Build());
}

void BeliefFactory::AddSensorBelief(FString key, USensor* sensor)
{
	//Beliefs.Add(key, )
}

void BeliefFactory::AddLocationBelief(FString key, float distance, FVector& locationCondition)
{
	Beliefs.Add(key, AgentBeliefs::Builder(key).WithCondition([=, this]() { return InRangeOf(locationCondition, distance); }).WithLocation([=]() {return locationCondition;}).Build());
}

//void BeliefFactory::AddLocationBelief(AGoapAgent& agent, FString key, float distance, const AActor* locationOfActor)
//{
//	AddLocationBelief(agent, key, distance, locationOfActor->GetActorLocation());
//}

bool BeliefFactory::InRangeOf(FVector position, float range)
{
	AActor* owner = Component ? Component->GetOwner() : nullptr;

	if (!owner)
	{
		return false;
	}
	
	return FVector::Dist(owner->GetActorLocation(), position) <= range;
}


