// Fill out your copyright notice in the Description page of Project Settings.


#include "RechargeStrategy.h"
#include "AI_Controller.h"
#include "NPC.h"


void URechargeStrategy::Initialize(AAI_Controller* inAI, float inGoal)
{
	AI = inAI;
	StaminaGoal = inGoal;
}

void URechargeStrategy::Start()
{
	NPC = Cast<ANPC>(AI->GetPawn());
	NPC->bRecharging = true;
}

void URechargeStrategy::Tick(float DeltaTime)
{
	if (!NPC)
	{
		return;
	}

	NPC->ModifyStat("Stamina", 5.f * DeltaTime);

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

