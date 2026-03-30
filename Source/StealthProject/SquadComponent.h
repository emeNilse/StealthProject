// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SquadSubsystem.h"
#include "SquadComponent.generated.h"

class UGoapComponent;
DECLARE_DELEGATE(FOnRequstComplete);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class STEALTHPROJECT_API USquadComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	USquadComponent();

	FOnRequstComplete OnComplete;

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

	AActor* GetSquadTarget();

	void EncounteredTarget(AActor* newTarget);

	void SquadStateChanged();

	void InjectSquadBeliefsToGoap();

	void RequestFlankingPosition(AAI_Controller* inAI);

	void OnFlankReady(FVector flankPosition);

private:
	TWeakObjectPtr<ASquadManager> SquadManagerID;

	UGoapComponent* GoapComponent;
		
};
