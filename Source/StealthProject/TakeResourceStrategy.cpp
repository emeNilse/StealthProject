
#include "TakeResourceStrategy.h"

void UTakeResourceStrategy::Start()
{
	Super::Start();

	if (Status == EStrategyStatus::Failed)
	{
		return;
	}

	Storage = Cast<IResourceStorage>(Target);

	if (!Storage)
	{
		Status = EStrategyStatus::Failed;
		return;
	}
	else
	{
		Storage->Take(Resource, Amount);
		Status = EStrategyStatus::Succeeded;
	}
}

void UTakeResourceStrategy::Stop()
{
}

bool UTakeResourceStrategy::CanPerform() const
{
	if (Storage)
	{
		return Storage->CanTake(Resource, Amount);
	}
	
	return false;
}

bool UTakeResourceStrategy::Complete() const
{
	return false;
}
