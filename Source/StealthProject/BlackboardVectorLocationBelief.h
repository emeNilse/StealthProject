
#pragma once

#include "CoreMinimal.h"
#include "GoapBelief.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BlackboardVectorLocationBelief.generated.h"


UCLASS()
class STEALTHPROJECT_API UBlackboardVectorLocationBelief : public UGoapBelief
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere)
	FName BlackboardVectorKeyName;

	UPROPERTY(EditAnywhere)
	float DistanceThreshold = 5.f;

	virtual bool Evaluate(AAI_Controller* AI) const override
	{
		if (!AI) return false;

		FVector position = AI->GetBlackboardComponent()->GetValueAsVector(BlackboardVectorKeyName);

		return FVector::DistSquared(AI->GetPawn()->GetActorLocation(), position) <= DistanceThreshold * DistanceThreshold;
	}
};
