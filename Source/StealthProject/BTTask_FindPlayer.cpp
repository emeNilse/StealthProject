// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_FindPlayer.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Actor.h"
#include "TimerManager.h"

UBTTask_FindPlayer::UBTTask_FindPlayer(FObjectInitializer const& ObjectInitializer)
{
	
}

EBTNodeResult::Type UBTTask_FindPlayer::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (auto* const Player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0))
	{
		FTimerHandle UnusedHandle;
		GetWorld()->GetTimerManager().SetTimer(UnusedHandle, 0.2, false);

		auto const PlayerLocation = Player->GetActorLocation();

		OwnerComp.GetBlackboardComponent()->SetValueAsVector(GetSelectedBlackboardKey(), PlayerLocation);

		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);

		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}