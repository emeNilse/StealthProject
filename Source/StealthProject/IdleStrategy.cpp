// Fill out your copyright notice in the Description page of Project Settings.


#include "IdleStrategy.h"

IdleStrategy::IdleStrategy(float duration) : bTimerComplete(false)
{
	Timer = MakeUnique<CountdownTimer>(duration);
	UE_LOG(LogTemp, Warning, TEXT("bRunning at construct ="), bTimerComplete ? TEXT("true") : TEXT("false"));
	/*Timer->OnTimerStart.AddLambda([this]() { bTimerComplete = false; });

	Timer->OnTimerStop.AddLambda([this]() { bTimerComplete = true; });*/
}

IdleStrategy::~IdleStrategy()
{
}

void IdleStrategy::Start()
{
	bTimerComplete = false;
	UE_LOG(LogTemp, Warning, TEXT("IdleStrategy start called"));
	UE_LOG(LogTemp, Warning, TEXT("bRunning at start ="), bTimerComplete ? TEXT("true") : TEXT("false"));
	Timer->Start();
}

void IdleStrategy::Tick(float DeltaTime)
{
	Timer->Tick(DeltaTime);
	//UE_LOG(LogTemp, Warning, TEXT("IdleStrategy Tick: Current=%.2f, bRunning=%d, dt = %.2f"), Timer->GetDuration(), Timer->IsRunning(), DeltaTime);

	if (Timer->IsFinished())
	{
		bTimerComplete = true;
		//UE_LOG(LogTemp, Warning, TEXT("IdleStrategy COMPLETE!"));
	}
}

void IdleStrategy::Stop()
{
	Timer->Stop();
	UE_LOG(LogTemp, Warning, TEXT("IdleStrategy stop called"));
	UE_LOG(LogTemp, Warning, TEXT("bRunning at stop first ="), bTimerComplete ? TEXT("true") : TEXT("false"));
	bTimerComplete = true;
	UE_LOG(LogTemp, Warning, TEXT("bRunning at stop end ="), bTimerComplete ? TEXT("true") : TEXT("false"));
}
