// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GoapAction.h"
#include "ActionStackComponent.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnActionStackFailed);
DECLARE_MULTICAST_DELEGATE(FOnActionStackFinished);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class STEALTHPROJECT_API UActionStackComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UActionStackComponent();

	FOnActionStackFailed OnStackFailed;
	FOnActionStackFinished OnStackFinished;

	UPROPERTY()
	AAI_Controller* OwnerAI;

	void PushAction(TSharedPtr<GoapAction> Action);

	void ClearStack();

	bool IsEmpty();

	void UpdateActions(float DeltaTime);

	void AbortCurrentAction();

	const TArray<TSharedPtr<GoapAction>>& GetStack() const { return ActionStack; }

	TSharedPtr<GoapAction> GetCurrentAction() const { return CurrentStackAction; }

	bool IsExecuting() const { return bIsExecuting; }

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:

	TArray<TSharedPtr<GoapAction>> ActionStack;
	TSet<TSharedPtr<GoapAction>> FirstTimeActions;
	TSharedPtr<GoapAction> CurrentStackAction;
	bool bIsExecuting;
};
