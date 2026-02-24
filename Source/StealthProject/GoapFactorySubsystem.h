
#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "GoapPlannerInterface.h"
#include "GoapPlanner.h"
#include "GoapFactorySubsystem.generated.h"

//I read somewhere that one should create a Goap Factory that keeps the Planner.
//Creates a unique pointer to a planner, so I guess this keeps the AI's planner safe.

UCLASS()
class STEALTHPROJECT_API UGoapFactorySubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:

	virtual void Initialize(FSubsystemCollectionBase& Collection) override
	{
		Super::Initialize(Collection);
	}

	TUniquePtr<IGoapPlannerInterface> CreatePlanner()
	{
		return MakeUnique<GoapPlanner>();
	}
};
