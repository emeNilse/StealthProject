
#pragma once

#include "CoreMinimal.h"
#include "GoapBelief.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BlackboardObjectBelief.generated.h"


UCLASS(Blueprintable, EditInlineNew)
class STEALTHPROJECT_API UBlackboardObjectBelief : public UGoapBelief
{
	GENERATED_BODY()
	
public:

	UPROPERTY(EditAnywhere)
	FName BlackboardKey;

	virtual bool Evaluate(AAI_Controller* AI) const override
	{
		if (!AI) return false;

		AActor* Target = Cast<AActor>(AI->GetBlackboardComponent()->GetValueAsObject(BlackboardKey));

		return Target ? true : false;
	}
};
