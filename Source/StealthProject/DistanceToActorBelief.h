
#pragma once

#include "CoreMinimal.h"
#include "GoapBelief.h"
#include "AI_Controller.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "DistanceToActorBelief.generated.h"

//Goap Belief that verifies if the player is within a certain distance to the AI.

UCLASS(Blueprintable, EditInlineNew)
class STEALTHPROJECT_API UDistanceToActorBelief : public UGoapBelief
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere)
	float MaxDistance;

	UPROPERTY(EditAnywhere)
	FName BlackboardKey;

	virtual bool Evaluate(AAI_Controller* AI) const override
	{
		if (!AI)
		{
			return false;
		}

		AActor* Target = Cast<AActor>(AI->GetBlackboardComponent()->GetValueAsObject(BlackboardKey));

		if (!Target)
		{
			return false;
		}

		return FVector::DistSquared(AI->GetPawn()->GetActorLocation(), Target->GetActorLocation()) <= MaxDistance * MaxDistance;
	}
};
