// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AgentBeliefs.h"

/**
 * 
 */
class STEALTHPROJECT_API GoapGoal
{
public:
	GoapGoal(FString name);
	~GoapGoal();

	FString Name;

	//FORCEINLINE const FString& GetName() const { return Name; }

	float Priority;

	//FORCEINLINE float GetPriority() const { return Priority; }

	TSet<TSharedPtr<AgentBeliefs>> DesiredEffects;


	class Builder
	{
		TSharedPtr<GoapGoal> goal;

	public:
		Builder(const FString name);

		Builder& WithPriority(float priority)
		{
			goal->Priority = priority;
			return *this;
		}

		Builder& WithDesiredEffect(AgentBeliefs effect)
		{
			goal->DesiredEffects.Add(MakeShared<AgentBeliefs>(effect));
			return *this;
		}

		TSharedPtr<GoapGoal> Build()
		{
			return goal;
		}

	};

};
