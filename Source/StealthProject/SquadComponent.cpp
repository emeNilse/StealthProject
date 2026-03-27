// Fill out your copyright notice in the Description page of Project Settings.


#include "SquadComponent.h"
#include "GoapComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

USquadComponent::USquadComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}

void USquadComponent::SetSquad(TWeakObjectPtr<ASquadManager> inManager)
{
	SquadManagerID = inManager;
	if (SquadManagerID.IsValid())
	{
		SquadManagerID->OnSquadStateChanged.AddUObject(this, &USquadComponent::SquadStateChanged);
	}
}

ESquadState USquadComponent::GetSquadState()
{
	if (SquadManagerID.IsValid())
	{
		return SquadManagerID->GetSquadState();
	}
	return ESquadState::Default;
}

AActor* USquadComponent::GetSquadTarget()
{
	if (SquadManagerID.IsValid())
	{
		return SquadManagerID->GetCurrentTarget();
	}
	
	return nullptr;
}

void USquadComponent::EncounteredTarget(AActor* newTarget)
{
	if (SquadManagerID.IsValid())
	{
		SquadManagerID->SquadMemberEncounteredTarget(newTarget);
	}
}

void USquadComponent::SquadStateChanged()
{

}

void USquadComponent::InjectSquadBeliefsToGoap()
{

}

void USquadComponent::RequestFlankingPosition(AAI_Controller* inAI)
{
	//SquadManagerID->CalculateFlankingPosition(this);

	FVector flankPosition = SquadManagerID->RequestFlankingPosition(inAI);

	GoapComponent->GetBlackboardData()->SetValueAsVector("ShootingPosition", flankPosition);
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

