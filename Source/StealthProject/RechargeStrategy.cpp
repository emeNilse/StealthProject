// Fill out your copyright notice in the Description page of Project Settings.


#include "RechargeStrategy.h"
#include "AI_Controller.h"
#include "NPC.h"

void URechargeStrategy::Start()
{
	NPC = Cast<ANPC>(AI->GetPawn());
	NPC->bRecharging = true;
	Status = EStrategyStatus::Running;
}

void URechargeStrategy::Tick(float DeltaTime)
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

	NPC->ModifyStat("Stamina", PowerDrainRate * DeltaTime);

	if (NPC->GetStat("Stamina") > StaminaGoal)
	{
		NPC->SetStat("Stamina", StaminaGoal);
	}
}

void URechargeStrategy::Stop()
{
	NPC->bRecharging = false;
}

bool URechargeStrategy::CanPerform() const
{
	return !Complete();
}

bool URechargeStrategy::Complete() const
{
	return NPC->GetStat("Stamina") >= StaminaGoal;
}

