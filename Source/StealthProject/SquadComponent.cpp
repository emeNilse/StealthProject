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
	GoapComponent->GetBlackboardData()->SetValueAsObject("SquadTarget", SquadManagerID->GetCurrentTarget());
}

void USquadComponent::RequestFlankingPosition(AAI_Controller* inAI)
{
	SquadManagerID->RequestFlankingPosition(inAI, FOnCalculationComplete::CreateUObject(this, &USquadComponent::OnFlankReady));

	/*FlankSide = SquadManagerID->RequestFlankingDirection(inAI);

	FEnvQueryRequest QueryRequest = FEnvQueryRequest(Query, this);

	FEnvNamedValue paramX;
	paramX.ParamName = "FlankDirectionX";
	paramX.Value = FlankSide.X;
	paramX.ParamType = EAIParamType::Float;
	FEnvNamedValue paramY;
	paramY.ParamName = "FlankDirectionY";
	paramY.Value = FlankSide.Y;
	paramY.ParamType = EAIParamType::Float;
	FEnvNamedValue paramZ;
	paramZ.ParamName = "FlankDirectionZ";
	paramZ.Value = FlankSide.Z;
	paramZ.ParamType = EAIParamType::Float;

	QueryRequest.SetNamedParam(paramX);
	QueryRequest.SetNamedParam(paramY);
	QueryRequest.SetNamedParam(paramZ);

	QueryRequest.Execute(EEnvQueryRunMode::AllMatching, this, &USquadComponent::FlankingQueryResult);*/
}

void USquadComponent::OnFlankReady(FVector flankPosition)
{
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

