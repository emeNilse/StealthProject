// Fill out your copyright notice in the Description page of Project Settings.


#include "IdleStrategy.h"

IdleStrategy::IdleStrategy(float duration)
{
	Timer = MakeUnique<CountdownTimer>(duration);

	Timer->OnTimerStart.AddLambda([this]() { bComplete = false; });

	Timer->OnTimerStop.AddLambda([this]() { bComplete = true; });
}

IdleStrategy::~IdleStrategy()
{
}

void IdleStrategy::Start()
{
	Timer->Start();
}

void IdleStrategy::Tick(float DeltaTime)
{
	Timer->Tick(DeltaTime);
}
