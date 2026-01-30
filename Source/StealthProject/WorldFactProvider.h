// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "UObject/Interface.h"
#include "WorldFactProvider.generated.h"

UENUM()
enum class EWorldFactType : uint8
{
	Bool,
	Int,
	Float
};

USTRUCT(BlueprintType)
struct FWorldFact
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName Key;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EWorldFactType Type;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool BoolValue;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 IntValue;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float FloatValue;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FVector Location;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TWeakObjectPtr<AActor> Source;
};

UINTERFACE(MinimalAPI)
class UWorldFactProvider : public UInterface
{
	GENERATED_BODY()
};


class STEALTHPROJECT_API IWorldFactProvider
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void GatherWorldFacts(UPARAM(ref) TArray<FWorldFact>& OutFacts);

};
