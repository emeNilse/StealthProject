// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"


DECLARE_MULTICAST_DELEGATE(FOnTimerEvent);
/**
 * 
 */
class STEALTHPROJECT_API CountdownTimer
{
	float Duration = 0.f;
	float CurrentDuration = 0.f;
	bool bRunning = false;

public:
	CountdownTimer(float duration);
	~CountdownTimer();

	/*FOnTimerEvent OnTimerStart;
	FOnTimerEvent OnTimerStop;*/
	
	void Start();

	void Tick(float DeltaTime);

	void Stop();

	void Reset();

	bool IsRunning() const { return bRunning; }

	bool IsFinished() const { return !bRunning && CurrentDuration <= 0.f; }

	float GetDuration() const { return CurrentDuration; }
};
