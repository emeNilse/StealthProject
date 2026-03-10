
#pragma once

#include "CoreMinimal.h"
#include "GoapBelief.h"
#include "GoapWorldStateComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "WorldStateFactBelief.generated.h"

//Goap Belief that is meant to verify info from the World Facts.
//Have yet to find a suitable use for it. Coming in Goap Presentation.

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

	UPROPERTY(EditAnywhere)
	FName BlackboardKey;

	bool CompareInt(int stat) const;

	bool CompareFloat(float stat) const;

	virtual bool Evaluate(AAI_Controller* AI) const override
	{
		UWorld* world = GetWorld();
		if (!world) return false;

		float BestDistance = FLT_MAX;

		TWeakObjectPtr<AActor> BestSource = nullptr;

		FVector AgentLocation = AI->GetPawn()->GetActorLocation();

		if (UGoapWorldStateComponent* WorldState = AI->GetOwner()->FindComponentByClass<UGoapWorldStateComponent>())
		{
			TArray<FWorldFact> facts = WorldState->GetFacts();

			for (const FWorldFact& fact : facts)
			{
				if (fact.Key == FactName)
				{
					float Distance = FVector::DistSquared(fact.Location, AgentLocation);
						
					switch (Type)
					{
					case EWorldFactType::Bool:
						if (fact.BoolValue == FactBool)
						{
							if (Distance < MaxRangeSquared && Distance < BestDistance)
							{
								BestDistance = Distance;
								BestSource = fact.Source;
							}
						}

					case EWorldFactType::Int:
						if (CompareInt(fact.IntValue))
						{
							if (Distance < MaxRangeSquared && Distance < BestDistance)
							{
								BestDistance = Distance;
								BestSource = fact.Source;
							}
						}

					case EWorldFactType::Float:
						if (CompareFloat(fact.FloatValue))
						{
							if (Distance < MaxRangeSquared && Distance < BestDistance)
							{
								BestDistance = Distance;
								BestSource = fact.Source;
							}
						}
					}
				}
			}
		}

		if (BestSource.IsValid())
		{
			AI->GetBlackboardComponent()->SetValueAsObject(BlackboardKey, BestSource.Get());
			return true;
		}
		return false;
	}
};
