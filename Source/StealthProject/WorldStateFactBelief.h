// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GoapBelief.h"
#include "GoapWorldStateComponent.h"

#include "WorldStateFactBelief.generated.h"

/**
 * 
 */
UCLASS()
class STEALTHPROJECT_API UWorldStateFactBelief : public UGoapBelief
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	FName FactName;

	UPROPERTY(EditAnywhere)
	EWorldFactType Type = EWorldFactType::Bool;

	UPROPERTY(EditAnywhere)
	bool FactBool;

	UPROPERTY(EditAnywhere)
	int IntValue;

	UPROPERTY(EditAnywhere)
	float FloatValue;

	UPROPERTY(EditAnywhere)
	float MaxRangeSquared;

	UPROPERTY(EditAnywhere)
	EFloatComparison Comparison = EFloatComparison::Less;

	bool CompareInt(int stat) const;

	bool CompareFloat(float stat) const;

	virtual bool Evaluate(AAI_Controller* AI) const override
	{
		UWorld* world = GetWorld();
		if (!world) return false;

		FVector AgentLocation = AI->GetPawn()->GetActorLocation();

		if (UGoapWorldStateComponent* WorldState = AI->GetOwner()->FindComponentByClass<UGoapWorldStateComponent>())
		{
			TArray<FWorldFact> facts = WorldState->GetFacts();

			for (const FWorldFact& fact : facts)
			{
				if (fact.Key == FactName)
				{
					if (FVector::DistSquared(fact.Location, AgentLocation) < MaxRangeSquared)
					{
						switch (Type)
						{
						case EWorldFactType::Bool:
							return fact.BoolValue == FactBool;
						case EWorldFactType::Int:
							return CompareInt(fact.IntValue);
						case EWorldFactType::Float:
							return CompareFloat(fact.FloatValue);
						}
					}
				}
			}
		}
		return false;
	}
};
