
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "UObject/Interface.h"
#include "WorldFactProvider.generated.h"

//Interface for any station or item that has World Facts to share with the AI.

UENUM(BlueprintType)
enum class EWorldFactType : uint8
{
	Default,
	Bool,
	Int,
	Float
};

UENUM(BlueprintType)
enum class EWorldBaseType : uint8
{
	Default,
	Station,
	Item
};

USTRUCT(BlueprintType)
struct FWorldFact
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FName Key = NAME_None;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool SingleOwner = false;

	bool IsAlreadyTaken = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	EWorldBaseType BaseType = EWorldBaseType::Default;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	EWorldFactType InfoType = EWorldFactType::Default;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (EditCondition = "Type == EWorldFactType::Bool", EditConditionHides))
	bool BoolValue = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (EditCondition = "Type == EWorldFactType::Int", EditConditionHides))
	int32 IntValue = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (EditCondition = "Type == EWorldFactType::Float", EditConditionHides))
	float FloatValue = 0.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FVector Location = FVector::ZeroVector;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TWeakObjectPtr<AActor> Source;

	TWeakObjectPtr<AActor> OwningActor;
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
