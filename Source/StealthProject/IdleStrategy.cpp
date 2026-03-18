
#include "IdleStrategy.h"

void UIdleStrategy::Start()
{
	Timer = MakeUnique<CountdownTimer>(Duration);
	UE_LOG(LogTemp, Warning, TEXT("bRunning at construct ="), bTimerComplete ? TEXT("true") : TEXT("false"));

	bTimerComplete = false;
	
	UE_LOG(LogTemp, Warning, TEXT("IdleStrategy start called"));
	UE_LOG(LogTemp, Warning, TEXT("bRunning at start ="), bTimerComplete ? TEXT("true") : TEXT("false"));
	Timer->Start();
	Status = EStrategyStatus::Running;
}

void UIdleStrategy::Tick(float DeltaTime)
{
	Timer->Tick(DeltaTime);

	if (Timer->IsFinished())
	{
		bTimerComplete = true;
		Status = EStrategyStatus::Succeeded;
	}
}

void UIdleStrategy::Stop()
{
	Timer->Stop();
	UE_LOG(LogTemp, Warning, TEXT("IdleStrategy stop called"));
	UE_LOG(LogTemp, Warning, TEXT("bRunning at stop first ="), bTimerComplete ? TEXT("true") : TEXT("false"));
	bTimerComplete = true;
	UE_LOG(LogTemp, Warning, TEXT("bRunning at stop end ="), bTimerComplete ? TEXT("true") : TEXT("false"));
	Status = EStrategyStatus::Succeeded;
}
