
#pragma once

#include "CoreMinimal.h"
#include "InteractionStrategy.h"
#include "ResourceStorage.h"
#include "CheckResourceStorageStrategy.generated.h"

//Not finished nor in use right now. This is for Goap presentation.
//Though cannot decide if this is unnecessary and simple interaction is enough.
UCLASS()
class STEALTHPROJECT_API UCheckResourceStorageStrategy : public UInteractionStrategy
{
	GENERATED_BODY()
	

public:
	UPROPERTY(EditAnywhere)
	FName Resource;

	virtual void Start() override;


};
