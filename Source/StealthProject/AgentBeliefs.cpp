// Fill out your copyright notice in the Description page of Project Settings.


#include "AgentBeliefs.h"

bool const UAgentBeliefs::Evaluate()
{
	return ConditionFunc ? ConditionFunc() : false;
}

FVector const UAgentBeliefs::GetLocation()
{
	return ObservedLocation ? ObservedLocation() : FVector::ZeroVector;
}

UAgentBeliefs::Builder::Builder(UObject* object, const FString& name)
{
	Beliefs = NewObject<UAgentBeliefs>(object);
	Beliefs->Name = name;
}




