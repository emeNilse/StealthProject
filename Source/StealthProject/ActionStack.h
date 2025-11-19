// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Containers/Array.h"
#include "Containers/Set.h"
#include "IAction.h"
#include "ActionStack.generated.h"



UCLASS()
class STEALTHPROJECT_API UActionStack : public UGameInstanceSubsystem, public FTickableGameObject
{
	GENERATED_BODY()

public:
	//UActionStack();
	~UActionStack();

	static UActionStack* Get(UWorld* World);

	virtual void Tick(float DeltaTime) override;
	virtual TStatId GetStatId() const override { RETURN_QUICK_DECLARE_CYCLE_STAT(UActionStack, STATGROUP_Tickables); }
	virtual bool IsTickable() const override { return true; }


	//UFUNCTION(BlueprintCallable)
	void PushAction(TSharedPtr<IActionInterface> Action);
	bool IsEmpty() const;

	const TArray<TSharedPtr<IActionInterface>>& GetStack() const { return ActionStack; }
	TSharedPtr<IActionInterface> GetCurrentAction() const { return CurrentAction; }

private:
	void UpdateActions();

private:
	TArray<TSharedPtr<IActionInterface>> ActionStack;
	TSet<IActionInterface*> FirstTimeSet;
	TSharedPtr<IActionInterface> CurrentAction;
};
