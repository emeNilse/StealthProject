// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_IncrementPathIndex.h"

#include "NPC.h"
#include "AI_Controller.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_IncrementPathIndex::UBTTask_IncrementPathIndex(FObjectInitializer const& ObjectInitializer) : 
	UBTTask_BlackboardBase(ObjectInitializer)
{

}

EBTNodeResult::Type UBTTask_IncrementPathIndex::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (auto* const Controller = Cast<AAI_Controller>(OwnerComp.GetAIOwner()))
	{
		if (auto* const NPC = Cast<ANPC>(Controller->GetPawn()))
		{
			if (auto* const BBC = OwnerComp.GetBlackboardComponent())
			{
				auto const NoOfPoints = NPC->GetPatrolPath()->Num();
				auto const MinIndex = 0;
				auto const MaxIndex = NoOfPoints - 1;
				auto Index = BBC->GetValueAsInt(GetSelectedBlackboardKey());

				//If directional mode is active
				if (bDirectional)
				{
					if (Index >= MaxIndex && Direction == EDirectionType::Forward)
					{
						Direction = EDirectionType::Reverse;
					}
					else if (Index == MinIndex && Direction == EDirectionType::Reverse)
					{
						Direction = EDirectionType::Forward;
					}
				}

				//increment value of index
				BBC->SetValueAsInt(GetSelectedBlackboardKey(), ((Direction == EDirectionType::Forward ? ++Index : --Index) % NoOfPoints));

				FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
				return EBTNodeResult::Succeeded;
			}
		}
	}
	
	return EBTNodeResult::Failed;
}
