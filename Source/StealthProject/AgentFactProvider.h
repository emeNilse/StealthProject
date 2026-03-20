// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "AgentFactProvider.generated.h"

UENUM(BlueprintType)
enum class ENPCState : uint8
{
	Calm,
	Investigative,
	Alert,
	Engaged
};



USTRUCT(BlueprintType)
struct FAgentInfo
{
	GENERATED_BODY()

	AActor* Agent;
	FVector Location = FVector::ZeroVector;
	ENPCState State = ENPCState::Calm;
	FString Task;
};

UINTERFACE(MinimalAPI)
class UAgentFactProvider : public UInterface
{
	GENERATED_BODY()
};


class STEALTHPROJECT_API IAgentFactProvider
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Agent Facts")
	void GatherAgentFacts(UPARAM(ref) TArray<FAgentInfo>& OutFacts);
};
