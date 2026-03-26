// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SquadSubsystem.h"
#include "SquadComponent.generated.h"

class UGoapComponent;
DECLARE_DELEGATE(FOnCalculationComplete);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class STEALTHPROJECT_API USquadComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	USquadComponent();

	FOnCalculationComplete OnComplete;

	UPROPERTY(BlueprintType, BlueprintReadWrite)
	ESquadRole SquadRole = ESquadRole::Default;

	AActor* CachedOwner;

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	TWeakObjectPtr<ASquadManager> GetSquad() const { return SquadManagerID; }

	UFUNCTION()
	void SetSquad(TWeakObjectPtr<ASquadManager> inManager);

	ESquadState GetSquadState();

	void EncounteredTarget(AActor* newTarget);

	void SquadStateChanged();

	void InjectSquadBeliefsToGoap();

	void RequestFlankingPosition();

	void FlankingCalculationComplete();

private:
	TWeakObjectPtr<ASquadManager> SquadManagerID;

	UGoapComponent* GoapComponent;
		
};
