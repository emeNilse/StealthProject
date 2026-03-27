
#pragma once

#include "CoreMinimal.h"
#include "GoapBelief.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AI_Controller.h"
#include "BlackboardObjectLocationBelief.generated.h"

//Beliefs based on Locations saved in the Blackboard
//A bit misleading, should've been called Blackboard Object Location Belief as it acquires the location of a saved object
UCLASS(Blueprintable, EditInlineNew)
class STEALTHPROJECT_API UBlackboardObjectLocationBelief : public UGoapBelief
{
	GENERATED_BODY()
	
public:

	UPROPERTY(EditAnywhere)
	FName BlackboardObjectKeyName;

	UPROPERTY(EditAnywhere)
	float DistanceThreshold = 5.f;

	virtual bool Evaluate(AAI_Controller* AI) const override
	{
		if (!AI) return false;

		AActor* Target = Cast<AActor>(AI->GetBlackboardComponent()->GetValueAsObject(BlackboardObjectKeyName));

		if (!Target) return false;

		return FVector::Dist(AI->GetPawn()->GetActorLocation(), Target->GetActorLocation()) <= DistanceThreshold;
	}
};
