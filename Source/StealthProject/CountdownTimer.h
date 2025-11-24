// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"


DECLARE_MULTICAST_DELEGATE(FOnTimerEvent);
/**
 * 
 */
class STEALTHPROJECT_API CountdownTimer
{
	float Duration;
	bool bRunning;

public:
	CountdownTimer(float duration);
	~CountdownTimer();

	FOnTimerEvent OnTimerStart;
	FOnTimerEvent OnTimerStop;

	void Start();

	void Tick(float DeltaTime);

	void Stop();
};
