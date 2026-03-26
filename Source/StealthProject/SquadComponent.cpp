// Fill out your copyright notice in the Description page of Project Settings.


#include "SquadComponent.h"
#include "GoapComponent.h"

USquadComponent::USquadComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}

void USquadComponent::SetSquad(TWeakObjectPtr<ASquadManager> inManager)
{
	SquadManagerID = inManager;
	if (SquadManagerID.IsValid())
	{
		SquadManagerID->OnFuckThisDelegate.AddUObject(this, &USquadComponent::SquadStateChanged);
	}
}

ESquadState USquadComponent::GetSquadState()
{
	
	
	return ESquadState();
}

void USquadComponent::EncounteredTarget(AActor* newTarget)
{
	SquadManagerID->SquadMemberEncounteredTarget(newTarget);
}

void USquadComponent::SquadStateChanged()
{

}

void USquadComponent::InjectSquadBeliefsToGoap()
{

}

void USquadComponent::RequestFlankingPosition()
{
	SquadManagerID->CalculateFlankingPosition(this);
}

void USquadComponent::FlankingCalculationComplete()
{
	OnComplete.ExecuteIfBound();
}

void USquadComponent::BeginPlay()
{
	Super::BeginPlay();
	CachedOwner = GetOwner();
	GoapComponent = CachedOwner->GetComponentByClass<UGoapComponent>();

	if (USquadSubsystem* Registry = GetWorld()->GetSubsystem<USquadSubsystem>())
	{
		Registry->Register(this);
	}
}

void USquadComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

