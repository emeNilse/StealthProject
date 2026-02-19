// Fill out your copyright notice in the Description page of Project Settings.


#include "AI_Controller.h"
#include "NPC.h"
#include "GameFramework/Controller.h"
#include "Perception/AISense_Sight.h"
#include "Perception/AISense_Hearing.h"
#include "Perception/AIPerceptionComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "StealthProjectCharacter.h"
#include "StealthGameState.h"
#include "GoapComponent.h"

AAI_Controller::AAI_Controller(FObjectInitializer const& ObjectInitializer)
{
	BlackboardComponent = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackboardComponent"));
	
	SetupPerceptionSystem();
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
		MyNPC = NPC;

		Goap = NPC->FindComponentByClass<UGoapComponent>();

		if (NPC->GetBlackBoardData())
		{
			UseBlackboard(NPC->GetBlackBoardData(), BlackboardComponent);
			
			APawn* OwnerPawn = GetPawn();
			if (UseBlackboard(NPC->GetBlackBoardData(), BlackboardComponent))
			{
				UE_LOG(LogTemp, Error, TEXT("Blackboard for %s"), *OwnerPawn->GetName());
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("Failed to get Blackboard for %s"), *OwnerPawn->GetName());
			}
			
		}
	}
}

void AAI_Controller::SetupPerceptionSystem()
{
	
	/*if (SightConfig)
	{*/
		SetPerceptionComponent(*CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("Perception Component")));

		SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight Config"));
		SightConfig->SightRadius = 500.f;
		SightConfig->LoseSightRadius = SightConfig->SightRadius + 250.f;
		SightConfig->PeripheralVisionAngleDegrees = 90.f;
		SightConfig->SetMaxAge(2.f);
		SightConfig->AutoSuccessRangeFromLastSeenLocation = 520.f;
		SightConfig->DetectionByAffiliation.bDetectEnemies = true;
		SightConfig->DetectionByAffiliation.bDetectFriendlies = true;
		SightConfig->DetectionByAffiliation.bDetectNeutrals = true;

		HearingConfig = CreateDefaultSubobject<UAISenseConfig_Hearing>(TEXT("HeaingConfig"));
		HearingConfig->HearingRange = 500.f;
		HearingConfig->LoSHearingRange = 400.f;
		HearingConfig->DetectionByAffiliation.bDetectEnemies = true;
		HearingConfig->DetectionByAffiliation.bDetectNeutrals = true;
		HearingConfig->DetectionByAffiliation.bDetectFriendlies = true;

		GetPerceptionComponent()->SetDominantSense(*SightConfig->GetSenseImplementation());
		GetPerceptionComponent()->OnTargetPerceptionUpdated.AddDynamic(this, &AAI_Controller::OnPerceptionUpdated);
		GetPerceptionComponent()->ConfigureSense(*SightConfig);
		GetPerceptionComponent()->ConfigureSense(*HearingConfig);
		
	//}
}

void AAI_Controller::OnPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
	if (Stimulus.Type == UAISense::GetSenseID<UAISense_Sight>())
	{
		OnTargetDetected(Actor, Stimulus);
	}
	else if (Stimulus.Type == UAISense::GetSenseID<UAISense_Hearing>() && !GetBlackboardComponent()->GetValueAsBool("bIgnoreNoise"))
	{
		OnNoiseHeard(Actor, Stimulus);
	}
}

void AAI_Controller::OnTargetDetected(AActor* Actor, FAIStimulus const Stimulus)
{
	if (auto* const c = Cast<AStealthProjectCharacter>(Actor))
	{
		if (Stimulus.WasSuccessfullySensed())
		{
			if (LineOfSightTo(Actor))
			{
				GetBlackboardComponent()->SetValueAsBool("bIgnoreNoise", true);
				GetBlackboardComponent()->SetValueAsObject("PlayerActor", Actor);
				GetBlackboardComponent()->SetValueAsBool("bCanSeePlayer", true);
				GetBlackboardComponent()->SetValueAsVector("PlayerLocation", c->GetActorLocation());
				MyNPC->SetNPCState(ENPCState::Engaged);
			}
		}
		else
		{
			GetBlackboardComponent()->SetValueAsBool("bIgnoreNoise", false);
			GetBlackboardComponent()->SetValueAsObject("PlayerActor", nullptr);
			GetBlackboardComponent()->SetValueAsBool("bCanSeePlayer", false);
			GetBlackboardComponent()->SetValueAsVector("PlayerLocation", c->GetActorLocation());

			MyNPC->SetNPCState(ENPCState::Alert);

			if (AStealthGameState* GS = GetWorld()->GetGameState<AStealthGameState>())
			{
				//Buggy, somewhere the bCanSeePlayer is still true
				//GS->SetGlobalAlert();
			}
		}
	}
}

void AAI_Controller::OnNoiseHeard(AActor* Actor, FAIStimulus const Stimulus)
{
	if (auto* const c = Cast<AStealthProjectCharacter>(Actor))
	{
		if (Stimulus.WasSuccessfullySensed())
		{
			GetBlackboardComponent()->SetValueAsBool("bHeardSomething", true);
			GetBlackboardComponent()->SetValueAsVector("NoiseLocation", Stimulus.StimulusLocation);
			MyNPC->SetNPCState(ENPCState::Investigative);
		}
		else
		{
			GetBlackboardComponent()->SetValueAsBool("bHeardSomething", false);
			GetBlackboardComponent()->SetValueAsVector("NoiseLocation", Stimulus.StimulusLocation);
		}
	}
}
