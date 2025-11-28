// Fill out your copyright notice in the Description page of Project Settings.


#include "AI_Controller.h"
#include "NPC.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AIPerceptionComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "StealthProjectCharacter.h"

AAI_Controller::AAI_Controller(FObjectInitializer const& ObjectInitializer)
{
	SetupPerceptionSystem();

	/*static ConstructorHelpers::FObjectFinder<UBlackboardData> BBAsset(TEXT("/StealthProject/Content/AI/Controller/BP_BlackboardData"));
	if (BBAsset.Succeeded())
	{
		BlackboardAsset = BBAsset.Object;
	}*/
}

void AAI_Controller::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	if (ANPC* const NPC = Cast<ANPC>(InPawn))
	{
		/*if (UBehaviorTree* const Tree = NPC->GetBehaviorTree())
		{
			UBlackboardComponent* b;
			UseBlackboard(Tree->BlackboardAsset, b);
			Blackboard = b;
			RunBehaviorTree(Tree);
		}*/

		if (NPC->GetBlackBoardData())
		{
			UseBlackboard(NPC->GetBlackBoardData(), BlackboardComponent);
		}
	}
}

void AAI_Controller::SetupPerceptionSystem()
{
	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight Config"));
	if (SightConfig)
	{
		SetPerceptionComponent(*CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("Perception Component")));
		SightConfig->SightRadius = 500.f;
		SightConfig->LoseSightRadius = SightConfig->SightRadius + 250.f;
		SightConfig->PeripheralVisionAngleDegrees = 90.f;
		SightConfig->SetMaxAge(5.f);
		SightConfig->AutoSuccessRangeFromLastSeenLocation = 520.f;
		SightConfig->DetectionByAffiliation.bDetectEnemies = true;
		SightConfig->DetectionByAffiliation.bDetectFriendlies = true;
		SightConfig->DetectionByAffiliation.bDetectNeutrals = true;

		GetPerceptionComponent()->SetDominantSense(*SightConfig->GetSenseImplementation());
		GetPerceptionComponent()->OnTargetPerceptionUpdated.AddDynamic(this, &AAI_Controller::OnTargetDetected);
		GetPerceptionComponent()->ConfigureSense(*SightConfig);
	}
}

void AAI_Controller::OnTargetDetected(AActor* Actor, FAIStimulus const Stimulus)
{
	if (auto* const c = Cast<AStealthProjectCharacter>(Actor))
	{
		GetBlackboardComponent()->SetValueAsBool("CanSeePlayer", Stimulus.WasSuccessfullySensed());
	}
}
