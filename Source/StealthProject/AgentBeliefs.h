// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
//#include "UObject/NoExportTypes.h"
//#include "AgentBeliefs.generated.h"

/**
 * 
 */
//UCLASS()
class STEALTHPROJECT_API AgentBeliefs /*: public UObject*/
{
	//GENERATED_BODY()
	

public:
	FString Name;

	TFunction<bool()> ConditionFunc;
	TFunction<FVector()> ObservedLocation;

	bool const Evaluate();

	bool Equals(TSharedPtr<AgentBeliefs> comparison);

	FVector const GetLocation();

	AgentBeliefs(FString name);


	class Builder
	{
		TSharedPtr<AgentBeliefs> Belief;

	public:
		Builder(/*UObject* object,*/const FString name);

		//I don't understand why this is what works, I tried my own way first but a youtube example and ChatGPT told me it had to be written like this
		Builder& WithCondition(TFunction<bool()> func)
		{
			Belief->ConditionFunc = func;
			return *this;
		}

		Builder& WithLocation(TFunction<FVector()> func)
		{
			Belief->ObservedLocation = func;
			return *this;
		}

		TSharedPtr<AgentBeliefs> Build()
		{
			return Belief;
		}

	};

};
