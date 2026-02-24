
#pragma once

#include "CoreMinimal.h"

//This CountdownTimer class was used by raw C++ classes.
//Originally when most of my Goap Scripts were just raw C++ and not UCLASSes, I had to create my own timer as they could use Unreal's timer.
DECLARE_MULTICAST_DELEGATE(FOnTimerEvent);

class STEALTHPROJECT_API CountdownTimer
{
public:
	float Duration = 0.f;

	float CurrentDuration = 0.f;

	bool bRunning = false;
	CountdownTimer(float duration);
	~CountdownTimer();

	
	void Start();

	void Tick(float DeltaTime);

	void Stop();

	void Reset();

	bool IsRunning() const { return bRunning; }

	bool IsFinished() const { return !bRunning && CurrentDuration <= 0.f; }

	float GetDuration() const { return CurrentDuration; }
};
