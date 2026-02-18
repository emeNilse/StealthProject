// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GoapBelief.h"
#include "Engine/OverlapResult.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"
#include "WithinRadiusBelief.generated.h"

//This class should rather be called PatrolRouteWithinRadius.
//At first I was think this could be multi-taskable,
//but to save the info it finds I don't think there's any other way. 
UCLASS(Blueprintable, EditInlineNew)
class STEALTHPROJECT_API UWithinRadiusBelief : public UGoapBelief
{
	GENERATED_BODY()
	

public: 
	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> ObjectOfInterestClass;

	UPROPERTY(EditAnywhere)
	FName BlackboardVariable;

	UPROPERTY(EditAnywhere)
	float SearchRadius = 2000.f;

	virtual bool Evaluate(AAI_Controller* AI) const override
	{
		if (!AI || !ObjectOfInterestClass) return false;

		UWorld* World = AI->GetWorld();
		if (!World) return false;

		FVector CurrentLocation = AI->GetPawn()->GetActorLocation();

		TArray<FOverlapResult> Overlaps;

		FCollisionShape Sphere = FCollisionShape::MakeSphere(SearchRadius);

		bool bHit = World->OverlapMultiByObjectType(Overlaps, CurrentLocation, FQuat::Identity, FCollisionObjectQueryParams(ECC_WorldDynamic), Sphere);

		for (const FOverlapResult& Result : Overlaps)
		{
			if (Result.GetActor()->IsA(ObjectOfInterestClass))
			{
				AI->GetBlackboardComponent()->SetValueAsObject(BlackboardVariable, Result.GetActor());
				
				return true;
			}
		}

		AI->GetBlackboardComponent()->SetValueAsObject(BlackboardVariable, nullptr);
		return false;
	}
};
