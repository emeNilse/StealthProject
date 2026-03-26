
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Info.h"
#include "GameFramework/Actor.h"
#include "AgentfactProvider.h"
#include "NPC.h"
#include "SquadConfigData.h"
#include "SquadManager.generated.h"

class USquadComponent;

DECLARE_DELEGATE(FOnCalculationComplete);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSquadStateChanged, AActor*, NewTarget);

DECLARE_MULTICAST_DELEGATE(FFuckThisDelegate);


UENUM(BlueprintType)
enum class ESquadRole : uint8
{
	Default		UMETA(DisplayName = "No Role Assigned"),
	Assualt		UMETA(DisplayName = "Assualt"),
	Support		UMETA(DisplayName = "Support"),
	Skirmisher	UMETA(DisplayName = "Skirmisher")
};

UENUM(BlueprintType)
enum class ESquadState : uint8
{
	Neutral,
	Alert,
	Combat
};

USTRUCT()
struct FFlankSlot
{
	GENERATED_BODY()

	FVector Position;

	TWeakObjectPtr<AAIController> User;

	bool bReserved = false;

	float Score = 0.f;
};



UCLASS()
class STEALTHPROJECT_API ASquadManager : public AInfo
{
	GENERATED_BODY()

public:
	ASquadManager();

	float Radius;

	float PlayerMoveThreshold = 200.f;

	FOnCalculationComplete OnCalculationComplete;

	FFuckThisDelegate OnFuckThisDelegate;

	UPROPERTY(BlueprintAssignable)
	FOnSquadStateChanged OnSquadStateChanged;

	void Initialize(TArray<TWeakObjectPtr<USquadComponent>> members);

	void ConfigInitialize(TWeakObjectPtr<USquadConfigData> configData);

	ESquadState GetSquadState() const { return SquadState; }

	UFUNCTION(BlueprintCallable)
	AActor* GetCurrentTarget() const { return CurrentTarget; }

	UFUNCTION()
	void FindMembers();

	UFUNCTION()
	void RoleAssignemnt();

	UFUNCTION()
	void NotifyMemberDied(USquadComponent* deadMember);

	void SquadMemberEncounteredTarget(AActor* newTarget);

	void ChangeState(ESquadState newState);

	void OnStateChange();

	void UpdateFlankSlots();

	bool ShouldUpdateFlankSlots();

	FVector RequestFlankingPosition(AAI_Controller* requester);

	void CalculateFlankingPosition(UObject* member);

	void FlankingQueryResult(TSharedPtr<FEnvQueryResult> result);

	FFlankSlot* FindBestAvailableSlot();

protected:
	virtual void BeginPlay() override;

private:
	FVector ManagerLocation;

	TArray<USquadComponent*> MySquad;

	ESquadState SquadState;

	AActor* CurrentTarget;

	FVector LastKnownTargetLocation;

	UWorld* CachedWorld;

	UEnvQuery* AnchorQuery; 

	TArray<FFlankSlot> FlankSlots;

	int AssualtRolesAvailable = 1;
	int SkirmisherRolesAvailable = 2;
	
};
