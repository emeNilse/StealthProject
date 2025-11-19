// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "MyGameInstanceSubsystem.generated.h"

UENUM(BlueprintType)
enum class EGlobalState : uint8
{
	Init,
	Menu,
	Play,
	Pause,
	Outro,
	Quit
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnGlobalStateChanged, EGlobalState, OldGlobalState, EGlobalState, NewGlobalState);


UCLASS()
class STEALTHPROJECT_API UMyGameInstanceSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

private:
	UPROPERTY()
	EGlobalState GlobalState = EGlobalState::Init;

public:
	UPROPERTY(BlueprintAssignable, Category = "GameState")
	FOnGlobalStateChanged OnGlobalStateChanged;
	
	UFUNCTION(BlueprintCallable)
	FORCEINLINE EGlobalState GetGlobalState() const { return GlobalState; }

	UFUNCTION(BlueprintCallable)
	void SetGlobalState(EGlobalState State);
};
