

#include "RechargeStation.h"
#include "GoapComponent.h"

void ARechargeStation::TickDrainPower(ANPC* NPC)
{
	if (UGoapActionStrategyBase* strategy = NPC->GetComponentByClass<UGoapComponent>()->GetCurrentStrategy())
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

