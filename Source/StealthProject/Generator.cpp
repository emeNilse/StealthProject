

#include "Generator.h"

void AGenerator::DrainPower(float drainrate)
{
	CurrentJuice -= drainrate;
	OnDrainPower();
}

void AGenerator::ReFuel(float fuelrate)
{
	CurrentJuice += fuelrate;
}
