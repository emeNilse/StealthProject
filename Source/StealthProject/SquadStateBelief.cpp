// Fill out your copyright notice in the Description page of Project Settings.


#include "SquadStateBelief.h"
#include "GoapComponent.h"

bool USquadStateBelief::Evaluate(AAI_Controller* AI) const
{
	if (!AI) return false;

	return AI->GetGoap()->GetSquadComponent()->GetSquadState() == DesiredSquadState;
}
