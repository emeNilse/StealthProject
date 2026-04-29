
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Info.h"
#include "GameFramework/Actor.h"
#include "AgentfactProvider.h"
#include "NPC.h"
#include "SquadConfigData.h"
#include "SquadManager.generated.h"

class USquadComponent;

DECLARE_DELEGATE_OneParam(FOnCalculationComplete, FVector);

DECLARE_MULTICAST_DELEGATE(FOnSquadStateChanged);


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
	Default,
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

USTRUCT()
struct FPositionsForRoleAssignment
{
	GENERATED_BODY()

	float DistanceToTarget;

	TWeakObjectPtr<USquadComponent> Member;
};

struct FPendingFlankRequest
{
	AAI_Controller* Requester;

	FOnCalculationComplete OnCalculationComplete;
};


UCLASS()
class STEALTHPROJECT_API ASquadManager : public AInfo
{
	GENERATED_BODY()

public:
	ASquadManager();

	float Radius;

	float PlayerMoveThreshold = 200.f;

	FOnSquadStateChanged OnSquadStateChanged;

	void Initialize(TArray<TWeakObjectPtr<USquadComponent>> members);

	void ConfigInitialize(TWeakObjectPtr<USquadConfigData> configData);

	ESquadState GetSquadState() const { return SquadState; }
	UFUNCTION(BlueprintCallable)
	AActor* GetCurrentTarget() const { return CurrentTarget; }

	TArray<FVector> GetChosenFlankPositions() const { return ChosenFlankPositions; }

	UFUNCTION()
	void FindMembers();

	UFUNCTION()
	void RoleAssignemnt();

	UFUNCTION()
	void NotifyMemberDied(USquadComponent* deadMember);

	void SquadMemberEncounteredTarget(AActor* newTarget);

	void ChangeState(ESquadState newState);

	void UpdateFlankSlots();

	bool ShouldUpdateFlankSlots();

	void RequestFlankingPosition(AAI_Controller* requester, FOnCalculationComplete callback);

	void FlankSlotWithoutUpdate();

	void RunCoverQuery();

	void CoverQueryResult(TSharedPtr<FEnvQueryResult> result);

	void PickASlot();

	FFlankSlot* FindBestAvailableSlot(AAI_Controller* requester);

	bool ItemAndMemberOnSameSide(FFlankSlot& slot, AAI_Controller* requester);

	bool VectorAngleDifference(FFlankSlot& slot1, FFlankSlot& slot2);

protected:
	virtual void BeginPlay() override;

private:
	FVector ManagerLocation;

	TArray<USquadComponent*> MySquad;

	ESquadState SquadState;

	AActor* CurrentTarget = nullptr;

	FVector LastKnownTargetLocation = FVector::ZeroVector;

	UWorld* CachedWorld;

	UEnvQuery* CoverQuery; 

	UEnvQuery* FlankQuery;

	TArray<FFlankSlot> FlankSlots;

	TArray<FFlankSlot> ReservedFlankSlots;

	TArray<FVector> ChosenFlankPositions;

	TArray<FPendingFlankRequest> PendingRequests;

	int AssualtRolesAvailable = 1;
	int SkirmisherRolesAvailable = 2;

	float LastFlankDirection = 0.f;
	
};
