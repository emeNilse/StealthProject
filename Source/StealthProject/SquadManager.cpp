// Fill out your copyright notice in the Description page of Project Settings.


#include "SquadManager.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "SquadComponent.h"

ASquadManager::ASquadManager()
{
	
}

void ASquadManager::Initialize(TArray<TWeakObjectPtr<USquadComponent>> members)
{
	for (TWeakObjectPtr<USquadComponent> squadMember : MySquad)
	{
		if (squadMember.IsValid())
		{
			MySquad.Add(squadMember.Get());
			squadMember->SetSquad(this);
		}
	}

	SquadState = ESquadState::Neutral;
	RoleAssignemnt();
}

void ASquadManager::FindMembers()
{
	TArray<TEnumAsByte<EObjectTypeQuery>> traceObjectTypes;
	traceObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn));

	TArray<AActor*> ignoreActors;
	ignoreActors.Init(this, 1);
	
	//UKismetSystemLibrary::SphereOverlapActors(GetWorld(), GetActorLocation(), Radius, traceObjectTypes, ANPC::StaticClass(), ignoreActors, MySquad);
}

void ASquadManager::RoleAssignemnt()
{
	for (USquadComponent* member : MySquad)
	{
		if (!member) continue;

		if (member->SquadRole != ESquadRole::Default)
		{
			continue;
		}
		else
		{
			if (AssualtRolesAvailable > 0)
			{
				member->SquadRole = ESquadRole::Assualt;
				AssualtRolesAvailable--;
			}
			else if (SkirmisherRolesAvailable > 0)
			{
				member->SquadRole = ESquadRole::Skirmisher;
				SkirmisherRolesAvailable--;
			}
			else
			{
				member->SquadRole = ESquadRole::Support;
			}
		}
	}
}

void ASquadManager::NotifyMemberDied(USquadComponent* deadMember)
{
	MySquad.Remove(deadMember);

	if (MySquad.Num() <= 1)
	{
		if (USquadSubsystem* system = GetWorld()->GetSubsystem<USquadSubsystem>())
		{
			system->RemoveSquad(this);
		}

		Destroy();
	}
}

//Manager should have world context so it can tell the AI to spread out/not stand in line to the player's pov
//Consider a "Bridge" system that handles environment query for the manager until ready to coonect to Unreal's EQS

void ASquadManager::ChangeState(ESquadState newState)
{
	SquadState = newState;
	OnStateChange();
}

void ASquadManager::OnStateChange()
{
	for (USquadComponent* member : MySquad)
	{

	}
}

void ASquadManager::BeginPlay()
{
	Super::BeginPlay();

}
