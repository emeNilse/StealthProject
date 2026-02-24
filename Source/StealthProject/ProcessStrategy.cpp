
#include "ProcessStrategy.h"
#include "AI_Controller.h"
#include "NPC.h"

void UProcessStrategy::Start()
{
	NPC = Cast<ANPC>(AI->GetPawn());
	
	Status = EStrategyStatus::Running;
}

void UProcessStrategy::Tick(float DeltaTime)
{
	if (!NPC)
	{
		Status = EStrategyStatus::Failed;
		return;
	}

	if (Complete())
	{
		Status = EStrategyStatus::Succeeded;
	}

	NPC->ModifyStat(StatName, StatDrainRate * DeltaTime);

	if (NPC->GetStat(StatName) > StatGoal)
	{
		NPC->SetStat(StatName, StatGoal);
	}
}

void UProcessStrategy::Stop()
{
}

bool UProcessStrategy::CanPerform() const
{
	return !Complete();
}

bool UProcessStrategy::Complete() const
{
	switch (Comparison)
	{
	case EProcessComparison::LessThan:
		return NPC->GetStat("StatName") < StatGoal;
	case EProcessComparison::LessThanOrEqualTo:
		return NPC->GetStat("StatName") <= StatGoal;
	case EProcessComparison::GreaterThan:
		return NPC->GetStat("StatName") > StatGoal;
	case EProcessComparison::GreaterThanOrEqualTo:
		return NPC->GetStat("StatName") >= StatGoal;
	case EProcessComparison::Equal:
		return NPC->GetStat("StatName") == StatGoal;
	}
	return false;
}
