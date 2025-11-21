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

FVector const AgentBeliefs::GetLocation()
{
	return ObservedLocation ? ObservedLocation() : FVector::ZeroVector;
}

AgentBeliefs::Builder::Builder(/*UObject* object, */const FString name) : Beliefs(MakeShared<AgentBeliefs>(name))
{}




