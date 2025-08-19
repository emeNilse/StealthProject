// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_FindPathPoint.h"

#include "NPC.h"
#include "AI_Controller.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_FindPathPoint::UBTTask_FindPathPoint(FObjectInitializer const& ObjectInitializer) :
	UBTTask_BlackboardBase(ObjectInitializer)
{

}

EBTNodeResult::Type UBTTask_FindPathPoint::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (auto* const Controller = Cast<AAI_Controller>(OwnerComp.GetAIOwner()))
	{
		if (auto* const BBC = OwnerComp.GetBlackboardComponent())
		{
			auto const Index = BBC->GetValueAsInt(GetSelectedBlackboardKey());

			if (auto* NPC = Cast<ANPC>(Controller->GetPawn()))
			{
				auto const Point = NPC->GetPatrolPath()->GetPatrolPoint(Index);

				auto const GlobalPoint = NPC->GetPatrolPath()->GetActorTransform().TransformPosition(Point);

				BBC->SetValueAsVector(PatrolPathVectorKey.SelectedKeyName, GlobalPoint);

				FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
				return EBTNodeResult::Succeeded;
			}
		}
	}
	
	return EBTNodeResult::Failed;
}
