// Fill out your copyright notice in the Description page of Project Settings.


#include "SquadRoleBelief.h"
#include "GoapComponent.h"

bool USquadRoleBelief::Evaluate(AAI_Controller* AI) const
{
	if (!AI) return false;

	return AI->GetGoap()->GetSquadComponent()->SquadRole == DesiredSquadRole;
}
