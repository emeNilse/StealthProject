
#include "InteractionStrategy.h"
#include "GoapWorldStateComponent.h"
#include "BehaviorTree/BlackboardComponent.h"


void UInteractionStrategy::Start()
{
	if (!AI)
	{
		Status = EStrategyStatus::Failed;
		return;
	}
	
	if (bEnableBlackboardLocation)
	{
		AActor* BlackboardTarget = Cast<AActor>(AI->GetBlackboardComponent()->GetValueAsObject(BlackboardKey));
		RuntimeTarget = BlackboardTarget;
	}
	else
	{
		RuntimeTarget = Target;
	}
	
	if (!RuntimeTarget || !RuntimeTarget->Implements<UInteractable>())
	{
		Status = EStrategyStatus::Failed;
		return;
	}

	bInteractionResult = IInteractable::Execute_Interact(RuntimeTarget, AI->GetPawn(), InteractionType);

	if (!bInteractionResult)
	{
		Status = EStrategyStatus::Failed;
	}
	else
	{
		Status = EStrategyStatus::Running;
	}
}

void UInteractionStrategy::Tick(float DeltaTime)
{
	if (Complete())
	{
		Status = EStrategyStatus::Succeeded;
	}
}

bool UInteractionStrategy::CanPerform() const
{
	return RuntimeTarget && RuntimeTarget->Implements<UInteractable>();
}

bool UInteractionStrategy::Complete() const
{
	return IInteractable::Execute_IsInteractionComplete(RuntimeTarget);
}
