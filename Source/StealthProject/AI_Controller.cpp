
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

//You will find traces of the BehaviourTree I originally used for testing.
//I'm keeping it in just in case I need to use it again.
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
		NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld());
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
	HearingConfig->HearingRange = 2000.f;
	HearingConfig->LoSHearingRange = 1000.f;
	HearingConfig->DetectionByAffiliation.bDetectEnemies = true;
	HearingConfig->DetectionByAffiliation.bDetectNeutrals = true;
	HearingConfig->DetectionByAffiliation.bDetectFriendlies = true;

	GetPerceptionComponent()->SetDominantSense(*SightConfig->GetSenseImplementation());
	GetPerceptionComponent()->OnTargetPerceptionUpdated.AddDynamic(this, &AAI_Controller::OnPerceptionUpdated);
	GetPerceptionComponent()->ConfigureSense(*SightConfig);
	GetPerceptionComponent()->ConfigureSense(*HearingConfig);
}

//Hearing should be ignored if the AI has spotted the player or just recently lost sight of player.
//If I had more time, I would delve deeper into the "Move to last known location" strategy, but
//for the scope of this game this had to do.
void AAI_Controller::OnPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
	if (Stimulus.Type == UAISense::GetSenseID<UAISense_Sight>())
	{
		OnTargetDetected(Actor, Stimulus);
	}
	else if (Stimulus.Type == UAISense::GetSenseID<UAISense_Hearing>() && !GetBlackboardComponent()->GetValueAsBool("bIgnoreNoise") && MyNPC->GetStat("Suspicious") < 90.f)
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
				GS->SetGlobalAlert();
			}
		}
	}
}

//If noise was heard in a spot that is unreachable via NavMesh -> Ignore Noise
void AAI_Controller::OnNoiseHeard(AActor* Actor, FAIStimulus const Stimulus)
{
	if (auto* const c = Cast<AStealthProjectCharacter>(Actor))
	{
		UNavigationPath* NavPath = NavSys->FindPathToLocationSynchronously(GetWorld(), GetPawn()->GetActorLocation(), Stimulus.StimulusLocation, GetPawn());

		if (!NavPath || !NavPath->IsValid() || NavPath->IsPartial())
		{
			UE_LOG(LogTemp, Error, TEXT("Failed to find path so heard nothing"));
			return;
		}
		if (Stimulus.WasSuccessfullySensed())
		{
			GetBlackboardComponent()->SetValueAsBool("bHeardSomething", true);
			GetBlackboardComponent()->SetValueAsBool("bHasInvestigated", false);
			GetBlackboardComponent()->SetValueAsVector("NoiseLocation", Stimulus.StimulusLocation);
			MyNPC->SetNPCState(ENPCState::Investigative);
		}
		else
		{
			//Not needed, I just got lost in debugging
			GetBlackboardComponent()->SetValueAsVector("NoiseLocation", Stimulus.StimulusLocation);
		}
	}
}
