// Fill out your copyright notice in the Description page of Project Settings.


#include "CountdownTimer.h"

CountdownTimer::CountdownTimer(float duration)
{
	Duration = duration;
	UE_LOG(LogTemp, Warning, TEXT("Duration at construct =%.2f"), Duration);
	CurrentDuration = duration;
	UE_LOG(LogTemp, Warning, TEXT("current at construct =%.2f"), CurrentDuration);
	bRunning = false;
	UE_LOG(LogTemp, Warning, TEXT("bRunning at construct ="), bRunning ? TEXT("true") : TEXT("false"));
}

CountdownTimer::~CountdownTimer()
{
}

void CountdownTimer::Start()
{
	CurrentDuration = Duration;
	UE_LOG(LogTemp, Warning, TEXT("Timer start called"));
	UE_LOG(LogTemp, Warning, TEXT("current at start =%.2f"), CurrentDuration);
	//OnTimerStart.Broadcast();

	bRunning = true;
	UE_LOG(LogTemp, Warning, TEXT("bRunning at start ="), bRunning ? TEXT("true") : TEXT("false"));
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
	UE_LOG(LogTemp, Warning, TEXT("timer stop called"));
	UE_LOG(LogTemp, Warning, TEXT("current at stop =%.2f"), CurrentDuration);
	UE_LOG(LogTemp, Warning, TEXT("bRunning at stop first ="), bRunning ? TEXT("true") : TEXT("false"));
	if (!bRunning)
	{
		return;
	}
	UE_LOG(LogTemp, Warning, TEXT("timer stop called and finished"));
	bRunning = false;
	UE_LOG(LogTemp, Warning, TEXT("bRunning at stop end ="), bRunning ? TEXT("true") : TEXT("false"));
	//OnTimerStop.Broadcast();
}

void CountdownTimer::Reset()
{
	Start();
}

