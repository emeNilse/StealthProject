// Fill out your copyright notice in the Description page of Project Settings.


#include "RechargeStrategy.h"
#include "AI_Controller.h"
#include "NPC.h"


RechargeStrategy::RechargeStrategy(AAI_Controller* inAI, float inGoal) : AI(inAI), StaminaGoal(inGoal)
{
}

RechargeStrategy::~RechargeStrategy()
{
}

void RechargeStrategy::Start()
{
	NPC = Cast<ANPC>(AI->GetPawn());
	NPC->bRecharging = true;

}

void RechargeStrategy::Tick(float DeltaTime)
{
	if (!NPC)
	{
		return;
	}

	NPC->Stamina += 5.f * DeltaTime;

	if (NPC->Stamina > StaminaGoal)
	{
		NPC->Stamina = StaminaGoal;
	}
}

void RechargeStrategy::Stop()
{
	NPC->bRecharging = false;
}

bool RechargeStrategy::CanPerform() const
{
	return !Complete();
}

bool RechargeStrategy::Complete() const
{
	return NPC->Stamina >= StaminaGoal;
}
