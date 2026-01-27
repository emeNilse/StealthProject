// Fill out your copyright notice in the Description page of Project Settings.


#include "Generator.h"

void AGenerator::DrainPower(float drainrate)
{
	CurrentJuice -= drainrate;
}
