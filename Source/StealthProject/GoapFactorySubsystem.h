// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "GoapPlannerInterface.h"
#include "GoapPlanner.h"
#include "GoapFactorySubsystem.generated.h"

/**
 * 
 */
UCLASS()
class STEALTHPROJECT_API UGoapFactorySubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:

	virtual void Initialize(FSubsystemCollectionBase& Collection) override
	{
		Super::Initialize(Collection);
	}

	TSharedPtr<IGoapPlannerInterface> CreatePlanner()
	{
		return MakeShared<GoapPlanner>();
	}
};
