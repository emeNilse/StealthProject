// Fill out your copyright notice in the Description page of Project Settings.


#include "CountdownTimer.h"

CountdownTimer::CountdownTimer(float duration)
{
	Duration = duration;

	bRunning = false;
}

CountdownTimer::~CountdownTimer()
{
}

void CountdownTimer::Start()
{
	CurrentDuration = Duration;
	
	OnTimerStart.Broadcast();

	bRunning = true;
}

void CountdownTimer::Tick(float DeltaTime)
{
	if (!bRunning)
	{
		return;
	}
	
	CurrentDuration -= DeltaTime;

	if (CurrentDuration <= 0)
	{
		Stop();
	}
}

void CountdownTimer::Stop()
{
	if (!bRunning)
	{
		return;
	}
	
	bRunning = false;
	OnTimerStop.Broadcast();
}

