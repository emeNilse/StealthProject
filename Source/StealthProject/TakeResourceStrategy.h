
#pragma once

#include "CoreMinimal.h"
#include "InteractionStrategy.h"
#include "ResourceStorage.h"
#include "TakeResourceStrategy.generated.h"

//Not in use yet. For Goap Presentation.

UCLASS(Blueprintable, EditInlineNew)
class STEALTHPROJECT_API UTakeResourceStrategy : public UInteractionStrategy
{
	GENERATED_BODY()
	
public:

	UPROPERTY(EditAnywhere)
	FName Resource;

	UPROPERTY(EditAnywhere)
	int32 Amount;

	IResourceStorage* Storage;

	virtual void Start() override;

	virtual void Stop() override;

	virtual bool CanPerform() const override;

	virtual bool Complete() const override;
};
