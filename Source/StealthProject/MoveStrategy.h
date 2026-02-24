
#pragma once

#include "CoreMinimal.h"
#include "GoapActionStrategyBase.h"
#include "AI_Controller.h"
#include "MoveStrategy.generated.h"

//All AI strategies that involve moving to a location uses the MoveStrategy.
//For the scope of the game project, I have limited the expanse of MoveStrategy, the target destination is fed in the editor.
// So for now, the movement startegy is rather "hard coded".
//I plan to make it more dynamic during graduation (or remake it entirely) so that it can be more dynamic and 
//find the closest Actor of Type TargetActor.
//And, eventually, to also see if said Actor (e.g. if it has a resource timer) is viable. 
// However, I am still trying to decide what is the best architecture for such a strategy.
UCLASS(Blueprintable, EditInlineNew)
class STEALTHPROJECT_API UMoveStrategy : public UGoapActionStrategyBase
{
	GENERATED_BODY()

public:
	
	virtual UGoapActionStrategyBase* CreateRunTimeInstance(UObject* Outer, AAI_Controller* inAI) const override
	{
		UMoveStrategy* Runtime = NewObject<UMoveStrategy>(Outer);
		Runtime->TargetActor = TargetActor;
		Runtime->AI = inAI;
		Runtime->WithinMinimumRange = WithinMinimumRange;
		return Runtime;
	}
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftObjectPtr <AActor> TargetActor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool DynamicRangeCostActive = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float WithinMinimumRange;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxStuckTime = 3.f;

	virtual void Start() override;

	virtual void Tick(float DeltaTime) override;

	virtual void Stop() override;

	virtual bool CanPerform() const override;
	virtual bool Complete() const override;

	virtual float GetCost(AAI_Controller* inAI, float DefaultCost) const override;

	float GetRemainingDistance(AAI_Controller* inAI, const FVector& targetDestination) const;

private:
	FVector Destination;

	AAI_Controller* AI;

	float StuckTimer;
};
