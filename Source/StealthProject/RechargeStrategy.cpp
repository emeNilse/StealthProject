// Fill out your copyright notice in the Description page of Project Settings.


#include "RechargeStrategy.h"
#include "AI_Controller.h"
#include "NPC.h"


//RechargeStrategy::RechargeStrategy(AAI_Controller* inAI, float inGoal) : AI(inAI), StaminaGoal(inGoal)
//{
//}
//
//RechargeStrategy::~RechargeStrategy()
//{
//}


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

	NPC->Stamina += 5.f * DeltaTime;

	if (NPC->Stamina > StaminaGoal)
	{
		NPC->Stamina = StaminaGoal;
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
	return NPC->Stamina >= StaminaGoal;
}

