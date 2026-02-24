

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
