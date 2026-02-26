

#include "PatrolPath.h"

APatrolPath::APatrolPath()
{
	PrimaryActorTick.bCanEverTick = false;
}

FVector APatrolPath::GetPatrolPoint(int const index) const
{
	return PatrolPoints[index];
}

int APatrolPath::Num() const
{
	return PatrolPoints.Num();
}

void APatrolPath::PathTaken()
{
	if (bTaken) return;

	bTaken = true;
}

void APatrolPath::NoLongerUsePath()
{
	if (!bTaken) return;

	bTaken = false;
}


