// Fill out your copyright notice in the Description page of Project Settings.


#include "RechargeStation.h"
#include "GoapComponent.h"

void ARechargeStation::TickDrainPower(ANPC* npc)
{
	if (UGoapActionStrategyBase* strategy = npc->GetComponentByClass<UGoapComponent>()->GetCurrentStrategy())
	{
		if (URechargeStrategy* recharge = Cast<URechargeStrategy>(strategy))
		{
			if (Generator)
			{
				Generator->DrainPower(recharge->GetPowerDrainRate());
			}
		}
	}
}

