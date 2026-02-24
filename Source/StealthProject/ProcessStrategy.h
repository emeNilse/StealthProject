
#pragma once

#include "CoreMinimal.h"
#include "GoapActionStrategyBase.h"
#include "ProcessStrategy.generated.h"

//Currently not in use, but the idea is for strategies to inherit from this strategy.
//I was thinking Recharge Strategy could inherit from this.

class AAI_Controller;
class ANPC;

UENUM()
enum class EProcessComparison : uint8
{
	LessThan,
	LessThanOrEqualTo,
	GreaterThan,
	GreaterThanOrEqualTo,
	Equal
};

UCLASS(Blueprintable, EditInlineNew)
class STEALTHPROJECT_API UProcessStrategy : public UGoapActionStrategyBase
{
	GENERATED_BODY()

public:
	virtual UGoapActionStrategyBase* CreateRunTimeInstance(UObject* Outer, AAI_Controller* inAI) const override
	{
		UProcessStrategy* Runtime = NewObject<UProcessStrategy>(Outer);
		Runtime->AI = inAI;
		Runtime->StatName = StatName;
		Runtime->StatGoal = StatGoal;
		Runtime->StatDrainRate = StatDrainRate;
		Runtime->Comparison = Comparison;
		return Runtime;
	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName StatName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float StatGoal;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float StatDrainRate = 10.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EProcessComparison Comparison;

	virtual void Start() override;

	virtual void Tick(float DeltaTime) override;

	virtual void Stop() override;

	virtual bool CanPerform() const override;

	virtual bool Complete() const override;

	float GetStatDrainRate() const { return StatDrainRate; }


private:

	AAI_Controller* AI;

	ANPC* NPC;

};
