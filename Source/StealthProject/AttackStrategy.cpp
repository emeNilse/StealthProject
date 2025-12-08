// Fill out your copyright notice in the Description page of Project Settings.


#include "AttackStrategy.h"

AttackStrategy::AttackStrategy(float duration)
{
	Timer = MakeUnique<CountdownTimer>(duration);

}

AttackStrategy::~AttackStrategy()
{
}

void AttackStrategy::Start()
{
	Timer->Start();
}

void AttackStrategy::Tick(float DeltaTime)
{
	Timer->Tick(DeltaTime);
}

void AttackStrategy::Stop()
{
	Timer->Stop();
}


