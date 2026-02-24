
#pragma once

#include "CoreMinimal.h"
#include "GoapBelief.h"
#include "LocationBelief.generated.h"

//Goap Belief for the location of an actor in the world

UCLASS(Blueprintable, EditInlineNew)
class STEALTHPROJECT_API ULocationBelief : public UGoapBelief
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere)
	float DistanceThreshold = 5.f;

	UPROPERTY(EditAnywhere)
	TSoftObjectPtr<AActor> TargetActor;

	virtual bool Evaluate(AAI_Controller* AI) const override
	{
		if (!AI || !TargetActor.IsValid())
		{
			return false;
		}

		return FVector::Dist(AI->GetPawn()->GetActorLocation(), TargetActor->GetActorLocation()) <= DistanceThreshold;
	}
	
};
