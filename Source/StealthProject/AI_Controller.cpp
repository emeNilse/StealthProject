// Fill out your copyright notice in the Description page of Project Settings.


#include "AI_Controller.h"
#include "NPC.h"

AAI_Controller::AAI_Controller(FObjectInitializer const& ObjectInitializer)
{

}

void AAI_Controller::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	if (ANPC* const npc = Cast<ANPC>(InPawn))
	{
		if (UBehaviorTree* const tree = npc->GetBehaviorTree())
		{
			UBlackboardComponent* b;
			UseBlackboard(tree->BlackboardAsset, b);
			Blackboard = b;
			RunBehaviorTree(tree);
		}
	}
}
