// Fill out your copyright notice in the Description page of Project Settings.


#include "NPC.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"

// Sets default values
ANPC::ANPC()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ANPC::BeginPlay()
{
	Super::BeginPlay();
	
}

void ANPC::RayCast()
{
	FHitResult* HitResult = new FHitResult();
	FVector forward = GetActorForwardVector();
	FVector StartTrace = GetActorLocation();
	FVector EndTrace = (forward * 500.f) + StartTrace;
	FCollisionQueryParams* CQP = new FCollisionQueryParams();
	CQP->AddIgnoredActor(this);

	DrawDebugLine(GetWorld(), StartTrace, EndTrace, FColor(255, 0, 0), true);

	if (GetWorld()->LineTraceSingleByChannel(*HitResult, StartTrace, EndTrace, ECC_Visibility, *CQP))
	{
		if (HitResult->GetActor() == UGameplayStatics::GetPlayerCharacter(GetWorld(), 0))
		{
			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, "Hit someone");
			
		}

		if (HitResult != NULL)
		{
			
			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, HitResult->GetActor()->GetFName().ToString());
			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, "Hit something");
			
		}
	}
}

UBehaviorTree* ANPC::GetBehaviorTree() const
{
	return Tree;
}

UBlackboardData* ANPC::GetBlackBoardData() const
{
	return BlackboardAsset;
}

APatrolPath* ANPC::GetPatrolPath()
{
	return PatrolPath;
}

// Called every frame
void ANPC::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//RayCast();

}

// Called to bind functionality to input
void ANPC::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

