
#include "CheckResourceStorageStrategy.h"

void UCheckResourceStorageStrategy::Start()
{
	Super::Start();

	if (Status == EStrategyStatus::Failed)
	{
		return;
	}

	IResourceStorage* storage = Cast<IResourceStorage>(Target);

	if (!storage)
	{
		Status = EStrategyStatus::Failed;
		return;
	}
	else
	{
		int32 Amount = storage->GetAmount(Resource);
	}

}
