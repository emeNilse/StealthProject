
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "UObject/Interface.h"
#include "WorldFactProvider.generated.h"

//Interface for any station or item that has World Facts to share with the AI.

UENUM(BlueprintType)
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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FName Key = NAME_None;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	EWorldFactType Type = EWorldFactType::Bool;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool BoolValue = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int32 IntValue = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float FloatValue = 0.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FVector Location = FVector::ZeroVector;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TWeakObjectPtr<AActor> Source;
};

UINTERFACE(BlueprintType)
class STEALTHPROJECT_API UWorldFactProvider : public UInterface
{
	GENERATED_BODY()
};


class STEALTHPROJECT_API IWorldFactProvider
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "World Facts")
	void GatherWorldFacts(UPARAM(ref) TArray<FWorldFact>& OutFacts);

};
