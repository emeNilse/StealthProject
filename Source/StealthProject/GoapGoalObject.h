// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GoapActionStrategyBase.h"
#include "GoapGoalObject.generated.h"


/**
 * 
 */
UCLASS(Blueprintable, EditInlineNew)
class STEALTHPROJECT_API UGoapGoalObject : public UObject
{
	GENERATED_BODY()

public:

	//GoapGoal

	//UPROPERTY(EditAnywhere, BlueprintReadWrite)
	//FString Name;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite)
	//int Priority;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite)
	//TArray<FString> DesiredEffects;

	////Not needed I think
	//void InitializeGoal(const FString& InName, float InPriority, const TArray<FString>& InDesiredEffects)
	//{
	//	Name = InName;
	//	Priority = InPriority;
	//	DesiredEffects = InDesiredEffects;
	//}


	//GoapAction

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Cost;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FString> PreConditions;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FString> Effects;

	UPROPERTY(EditAnywhere, Instanced, BlueprintReadWrite)
	UGoapActionStrategyBase* StrategyInstance;
	
};
