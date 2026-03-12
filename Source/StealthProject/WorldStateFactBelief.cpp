

#include "WorldStateFactBelief.h"

bool UWorldStateFactBelief::CompareInt(int stat) const
{
	switch (Comparison)
	{
	case EFloatComparison::Less:
		return stat < IntValue;
	case EFloatComparison::LessOrEqual:
		return stat <= IntValue;
	case EFloatComparison::Greater:
		return stat > IntValue;
	case EFloatComparison::GreaterOrEqual:
		return stat >= IntValue;
	case EFloatComparison::Equal:
		return stat == IntValue;
	}
	return false;
}

bool UWorldStateFactBelief::CompareFloat(float stat) const
{
	switch (Comparison)
	{
	case EFloatComparison::Less:
		return stat < FloatValue;
	case EFloatComparison::LessOrEqual:
		return stat <= FloatValue;
	case EFloatComparison::Greater:
		return stat > FloatValue;
	case EFloatComparison::GreaterOrEqual:
		return stat >= FloatValue;
	case EFloatComparison::Equal:
		return stat == FloatValue;
	}
	return false;
}

bool UWorldStateFactBelief::Evaluate(AAI_Controller* AI) const
{
	if (!AI) return false;
	
	UWorld* world = GetWorld();
	if (!world) return false;

	float BestDistance = FLT_MAX;

	TWeakObjectPtr<AActor> BestSource = nullptr;

	APawn* Pawn = AI->GetPawn();

	if (!IsValid(Pawn))
	{
		UE_LOG(LogTemp, Error, TEXT("Pawn invalid"));
		return false;
	}

	FVector AgentLocation = Pawn->GetActorLocation();

	UGoapComponent* GoapComp = Pawn->FindComponentByClass<UGoapComponent>();

	if (!IsValid(GoapComp))
	{
		UE_LOG(LogTemp, Error, TEXT("Goap invalid"));
		return false;
	}

	UGoapWorldStateComponent* ThisWorldState = GoapComp->WorldState;

	if (!IsValid(ThisWorldState))
	{
		UE_LOG(LogTemp, Error, TEXT("This World State is null"));
		return false;
	}

	TArray<FWorldFact> facts = ThisWorldState->GetFacts();

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
				break;

			case EWorldFactType::Int:
				if (CompareInt(fact.IntValue))
				{
					if (Distance < MaxRangeSquared && Distance < BestDistance)
					{
						BestDistance = Distance;
						BestSource = fact.Source;
					}
				}
				break;

			case EWorldFactType::Float:
				if (CompareFloat(fact.FloatValue))
				{
					if (Distance < MaxRangeSquared && Distance < BestDistance)
					{
						BestDistance = Distance;
						BestSource = fact.Source;
					}
				}
				break;
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
