// Fill out your copyright notice in the Description page of Project Settings.


#include "AgentBeliefs.h"

AgentBeliefs::AgentBeliefs(FString name)
{
	Name = name;
}

bool const AgentBeliefs::Evaluate()
{
	return ConditionFunc ? ConditionFunc() : false;
}

bool AgentBeliefs::Equals(TSharedPtr<AgentBeliefs> comparison)
{
	return (this->Name == comparison->Name);
}

FVector const AgentBeliefs::GetLocation()
{
	return ObservedLocation ? ObservedLocation() : FVector::ZeroVector;
}

AgentBeliefs::Builder::Builder(/*UObject* object, */const FString name) : Belief(MakeShared<AgentBeliefs>(name))
{
	//Belief = new AgentBeliefs(name);
}




