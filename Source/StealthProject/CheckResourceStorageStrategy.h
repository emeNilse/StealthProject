
#pragma once

#include "CoreMinimal.h"
#include "InteractionStrategy.h"
#include "ResourceStorage.h"
#include "CheckResourceStorageStrategy.generated.h"

//Not finished. This is for Goap presentation.

UCLASS()
class STEALTHPROJECT_API UCheckResourceStorageStrategy : public UInteractionStrategy
{
	GENERATED_BODY()
	

public:
	UPROPERTY(EditAnywhere)
	FName Resource;

	virtual void Start() override;


};
