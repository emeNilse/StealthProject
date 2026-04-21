// Fill out your copyright notice in the Description page of Project Settings.


#include "SquadManager.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "SquadComponent.h"
#include "StealthProjectCharacter.h"

ASquadManager::ASquadManager()
{
	
}

void ASquadManager::Initialize(TArray<TWeakObjectPtr<USquadComponent>> members)
{
	for (TWeakObjectPtr<USquadComponent> squadMember : members)
	{
		if (squadMember.IsValid())
		{
			MySquad.Add(squadMember.Get());
			squadMember->SetSquad(this);
		}
	}

	SquadState = ESquadState::Neutral;
}

void ASquadManager::ConfigInitialize(TWeakObjectPtr<USquadConfigData> configData)
{
	AnchorQuery = configData->AnchorQuery;
}

void ASquadManager::FindMembers()
{
	TArray<TEnumAsByte<EObjectTypeQuery>> traceObjectTypes;
	traceObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn));

	TArray<AActor*> ignoreActors;
	ignoreActors.Init(this, 1);
	
	//UKismetSystemLibrary::SphereOverlapActors(GetWorld(), GetActorLocation(), Radius, traceObjectTypes, ANPC::StaticClass(), ignoreActors, MySquad);
}

void ASquadManager::RoleAssignemnt()
{
	TArray<FPositionsForRoleAssignment> roleList;
	FVector targetLocation = CurrentTarget->GetActorLocation();

	for (USquadComponent* member : MySquad)
	{
		if (member->SquadRole != ESquadRole::Default)
		{
			continue;
		}
		
		FPositionsForRoleAssignment assignment;
		assignment.Member = member;
		assignment.DistanceToTarget = FVector::Dist(targetLocation, member->CachedOwner->GetActorLocation());

		roleList.Add(assignment);
	}
	
	roleList.Sort([](const FPositionsForRoleAssignment& A, const FPositionsForRoleAssignment& B)
		{
			return A.DistanceToTarget < B.DistanceToTarget;
		});

	for (FPositionsForRoleAssignment assignee : roleList)
	{
		if (AssualtRolesAvailable > 0)
		{
			assignee.Member->SquadRole = ESquadRole::Assualt;
			AssualtRolesAvailable--;
		}
		else if (SkirmisherRolesAvailable > 0)
		{
			assignee.Member->SquadRole = ESquadRole::Skirmisher;
			SkirmisherRolesAvailable--;
		}
		else
		{
			assignee.Member->SquadRole = ESquadRole::Support;
		}
	}

	/*for (USquadComponent* member : MySquad)
	{
		if (!member) continue;

		if (member->SquadRole != ESquadRole::Default)
		{
			continue;
		}
		else
		{
			if (AssualtRolesAvailable > 0)
			{
				member->SquadRole = ESquadRole::Assualt;
				AssualtRolesAvailable--;
			}
			else if (SkirmisherRolesAvailable > 0)
			{
				member->SquadRole = ESquadRole::Skirmisher;
				SkirmisherRolesAvailable--;
			}
			else
			{
				member->SquadRole = ESquadRole::Support;
			}
		}
	}*/
}

void ASquadManager::NotifyMemberDied(USquadComponent* deadMember)
{
	MySquad.Remove(deadMember);

	if (MySquad.Num() <= 1)
	{
		if (USquadSubsystem* system = GetWorld()->GetSubsystem<USquadSubsystem>())
		{
			system->RemoveSquad(this);
		}

		Destroy();
	}
}

void ASquadManager::SquadMemberEncounteredTarget(AActor* newTarget)
{
	if (!newTarget) return;

	if (auto* const c = Cast<AStealthProjectCharacter>(newTarget))
	{
		if (CurrentTarget != newTarget)
		{
			CurrentTarget = newTarget;
			RoleAssignemnt();
			ChangeState(ESquadState::Combat);
		}
	}
}

//Manager should have world context so it can tell the AI to spread out/not stand in line to the player's pov
//Consider a "Bridge" system that handles environment query for the manager until ready to coonect to Unreal's EQS

void ASquadManager::ChangeState(ESquadState newState)
{
	SquadState = newState;
	OnSquadStateChanged.Broadcast();
	//OnStateChange();
}

void ASquadManager::OnStateChange()
{
	for (USquadComponent* member : MySquad)
	{

	}
}

void ASquadManager::UpdateFlankSlots()
{
	FlankSlots.Empty();
	ChosenFlankPositions.Empty();

	//Query calculations and fill a new FlankSlots
	FEnvQueryRequest QueryRequest = FEnvQueryRequest(AnchorQuery, this);
	QueryRequest.Execute(EEnvQueryRunMode::AllMatching, this, &ASquadManager::FlankingQueryResult);
}

bool ASquadManager::ShouldUpdateFlankSlots()
{
	if (CurrentTarget == nullptr) return false;

	if (!IsValid(CurrentTarget)) return false;

	float distanceMoved = FVector::DistSquared(CurrentTarget->GetActorLocation(), LastKnownTargetLocation);
	
	return distanceMoved >= PlayerMoveThreshold * PlayerMoveThreshold;
}

void ASquadManager::RequestFlankingPosition(AAI_Controller* requester, FOnCalculationComplete callback)
{
	PendingRequests.Add({ requester, callback });
	
	if (FlankSlots.IsEmpty() || ShouldUpdateFlankSlots())
	{
		UpdateFlankSlots();
	}
	
	//What in Slot Update is not needed?
}

void ASquadManager::CalculateFlankingPosition(UObject* member)
{
	FEnvQueryRequest AnchorQueryRequest = FEnvQueryRequest(AnchorQuery, member);
	AnchorQueryRequest.Execute(EEnvQueryRunMode::SingleResult, this, &ASquadManager::FlankingQueryResult);
}

void ASquadManager::FlankingQueryResult(TSharedPtr<FEnvQueryResult> result)
{
	UE_LOG(LogTemp, Warning, TEXT("EQS Query finished! Valid: %d Items: %d"), result.IsValid(), result->Items.Num());
	
	if (!result.IsValid() || result->Items.Num() == 0) return;

	for (int i = 0; i < result->Items.Num(); i++)
	{
		FFlankSlot slot;
		slot.Position = result->GetItemAsLocation(i);
		slot.Score = result->GetItemScore(i);
		FlankSlots.Add(slot);
	}

	FlankSlots.Sort([](const FFlankSlot& A, const FFlankSlot& B) {return A.Score > B.Score;});

	for (FPendingFlankRequest& request : PendingRequests)
	{
		/*FVector RightSide = CurrentTarget->GetActorRightVector();
		FVector TargetToNPC = (CurrentTarget->GetActorLocation() - request.Requester->GetMyNPC()->GetActorLocation()).GetSafeNormal();
		float FlankDir = FVector::DotProduct(RightSide, TargetToNPC);*/
		
		FFlankSlot* slot = FindBestAvailableSlot(request.Requester);

		FVector pos = slot ? slot->Position : FVector::ZeroVector;

		if (slot)
		{
			slot->bReserved = true;
			slot->User = request.Requester;
			ChosenFlankPositions.Add(pos);
		}

		if (CurrentTarget)
		{
			LastKnownTargetLocation = CurrentTarget->GetActorLocation();
		}

		request.OnCalculationComplete.ExecuteIfBound(pos);
	}

	PendingRequests.Empty();
}

FFlankSlot* ASquadManager::FindBestAvailableSlot(AAI_Controller* requester)
{
	FFlankSlot* best = nullptr;
	float  bestScore = -FLT_MAX;

	//FlankSlots should be sorted in descending order
	for (FFlankSlot& slot : FlankSlots)
	{
		if (slot.bReserved) continue;

		if (ItemAndMemberOnSameSide(slot, requester))
		{
			best = &slot;
			return best;
		}
		/*if (slot.Score > bestScore)
		{
			bestScore = slot.Score;
			best = &slot;
		}*/
	}

	return best;
}

bool ASquadManager::ItemAndMemberOnSameSide(FFlankSlot& slot, AAI_Controller* requester)
{
	FVector rightSide = CurrentTarget->GetActorRightVector();
	FVector targetLocation = CurrentTarget->GetActorLocation();
	float itemToTargetDot = FVector::DotProduct(rightSide, (slot.Position - targetLocation));
	float requesterToTargetDot = FVector::DotProduct(rightSide, (requester->GetMyNPC()->GetActorLocation() - targetLocation));
	
	return (itemToTargetDot <= 0 && requesterToTargetDot <= 0 || itemToTargetDot > 0 && requesterToTargetDot > 0);
}

void ASquadManager::BeginPlay()
{
	Super::BeginPlay();

	CachedWorld = GetWorld();
}
