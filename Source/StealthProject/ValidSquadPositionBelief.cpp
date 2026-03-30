// Fill out your copyright notice in the Description page of Project Settings.


#include "ValidSquadPositionBelief.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GoapComponent.h"
#include "SquadComponent.h"

bool UValidSquadPositionBelief::Evaluate(AAI_Controller* AI) const
{
	if (!AI) return false;

	if (!AI->GetBlackboardComponent()->GetValueAsObject("SquadTarget")) return false;

	return (AI->GetBlackboardComponent()->GetValueAsVector(BlackboardKey) != FVector::ZeroVector && !AI->GetGoap()->GetSquadComponent()->GetSquad()->ShouldUpdateFlankSlots());
}