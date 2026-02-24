
#pragma once

#include "CoreMinimal.h"
#include "GoapActionStrategyBase.h"
#include "AI_Controller.h"
#include "NPC.h"
#include "PatrolPath.h"
#include "PatrolStrategy.generated.h"

//Action Strategy for going on Patrol. AI finds the nearest patrol path and then patrols along the route.

UCLASS(Blueprintable, EditInlineNew)
class STEALTHPROJECT_API UPatrolStrategy : public UGoapActionStrategyBase
{
	GENERATED_BODY()

public:
	virtual UGoapActionStrategyBase* CreateRunTimeInstance(UObject* Outer, AAI_Controller* inAI) const override
	{
		UPatrolStrategy* Runtime = NewObject<UPatrolStrategy>(Outer);
		Runtime->AI = inAI;
		return Runtime;
	}

	virtual void Start() override;

	virtual void Tick(float DeltaTime) override;

	virtual void Stop() override;

	virtual bool CanPerform() const override { return !Complete(); };

	virtual bool Complete() const override;


private:
	AAI_Controller* AI;

	UWorld* World;

	ANPC* NPC;

	FVector GlobalPoint;

	UBlackboardComponent* BBC;

	APatrolPath* MyPatrolPath;

	float DistanceToTarget;

	int Index;

	int IndexCounter;

	bool bPatrolling;

	int NOofPoints;

	APatrolPath* FindClosestPatrolPath(UWorld* inWorld, const FVector& inNPCLocation);

	int FindClostestPatrolPathPoint(APatrolPath* inPath, const FVector& inNPCLocation);

	float GetRemainingDistance(AAI_Controller* inAI, const FVector& targetDestination) const;

};
