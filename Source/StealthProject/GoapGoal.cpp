
#include "GoapGoal.h"

GoapGoal::GoapGoal(FString name)
{
	Name = name;
}

GoapGoal::~GoapGoal()
{
}

GoapGoal::Builder::Builder(const FString name)
{
	goal = MakeShared<GoapGoal>(name);
}
