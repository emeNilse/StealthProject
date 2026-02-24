
#include "StatBelief.h"

const float UStatBelief::GetStatValue(APawn* pawn) const
{
	if (const ANPC* StatOwner = Cast<ANPC>(pawn))
	{
		return StatOwner->GetStat(StatName);
	}

	return 0.0f;
}

bool UStatBelief::Compare(float stat) const
{
	switch (Comparison)
	{
	case EFloatComparison::Less:
		return stat < Value;
	case EFloatComparison::LessOrEqual:
		return stat <= Value;
	case EFloatComparison::Greater:
		return stat > Value;
	case EFloatComparison::GreaterOrEqual:
		return stat >= Value;
	case EFloatComparison::Equal:
		return stat == Value;
	case EFloatComparison::Range:
		return stat > Value && stat < MaxRangeValue;
	}
	return false;
}