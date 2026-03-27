
#pragma once

#include "CoreMinimal.h"
#include "GoapBelief.h"
#include "GoapComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "LookingAtObjectBelief.generated.h"


UCLASS(Blueprintable, EditInlineNew)
class STEALTHPROJECT_API ULookingAtObjectBelief : public UGoapBelief
{
	GENERATED_BODY()
	
public:

	UPROPERTY(EditAnywhere)
	FName BlackboardObjectKeyName;

	float AimThreshold = 0.8f;

	virtual bool Evaluate(AAI_Controller* AI) const override
	{
		UObject* Target = AI->GetBlackboardComponent()->GetValueAsObject(BlackboardObjectKeyName);
		AActor* TargetActor = Cast<AActor>(Target);

		//Temp
		TargetActor = AI->GetGoap()->GetSquadComponent()->GetSquadTarget();

		if (!TargetActor) return false;

		FVector ToTarget = (TargetActor->GetActorLocation() - AI->GetPawn()->GetActorLocation()).GetSafeNormal();
		return FVector::DotProduct(AI->GetPawn()->GetActorForwardVector(), ToTarget) > AimThreshold;
	}
};
