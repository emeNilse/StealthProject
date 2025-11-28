// Fill out your copyright notice in the Description page of Project Settings.


#include "GoapComponent.h"
#include "Navigation/PathFollowingComponent.h"
#include "BeliefFactory.h"



// Sets default values for this component's properties
UGoapComponent::UGoapComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UGoapComponent::BeginPlay()
{
	Super::BeginPlay();

	AI = Cast<AAI_Controller>(GetOwner()->GetInstigatorController());

	if (GetWorld())
	{
		GoapFactory = GetWorld()->GetGameInstance()->GetSubsystem<UGoapFactorySubsystem>();

		if (GoapFactory)
		{
			GoapPlanner = GoapFactory->CreatePlanner();
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("GoapFactorySubsystem is missing"));
		}
	}
}


// Called every frame
void UGoapComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UGoapComponent::SetupTimers()
{
	
}

void UGoapComponent::SetupBeliefs()
{
	BeliefFactory Factory = BeliefFactory(this, Beliefs);

	Factory.AddBelief("Nothing", []() { return false; });

	Factory.AddBelief("AgentIdle", [this]()
		{
			if (!AI) return false;
			return AI->GetMoveStatus() == EPathFollowingStatus::Idle;
		});

	Factory.AddBelief("AgentMoving", [this]()
		{
			if (!AI) return false;
			return AI->GetMoveStatus() == EPathFollowingStatus::Moving;
		});

	Factory.AddBelief("AgentStaminaLow", [this]() {return Stamina < 10.0;});



	Factory.AddLocationBelief("AgentAtOilWell", 3.f, OilWell);


}

void UGoapComponent::SetupAction()
{

}

void UGoapComponent::SetupGoals()
{

}

