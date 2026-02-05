// Fill out your copyright notice in the Description page of Project Settings.


#include "NPC.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"

// Sets default values
ANPC::ANPC()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	/*GetCharacterMovement()->bOrientRotationToMovement = false;
	GetCharacterMovement()->bUseControllerDesiredRotation = false;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 360.f, 0.f);*/
}

// Called when the game starts or when spawned
void ANPC::BeginPlay()
{
	Super::BeginPlay();

	//Stamina = 100.f;
	StatTimerInterval = 1.f;
	StatTimerRemaining = 1.f;

	GlobalDestination = GetActorLocation();
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

void ANPC::SetPatrolPath(APatrolPath* inPath)
{
	PatrolPath = inPath;
}

void ANPC::UpdateStats()
{
	if (!bRecharging)
	{
		//Stamina -= 5.f;
		ModifyStat("Stamina", -5.f);
	}

	//Stamina = FMath::Clamp(Stamina, 0, 100);

	UE_LOG(LogTemp, Warning, TEXT("Stamina %f"), GetStat("Stamina"));
}

void ANPC::SetDestination(FVector TargetDestination)
{
	GlobalDestination = TargetDestination;
}

float ANPC::GetStat(FName StatName) const
{
	if (const float* Value = Stats.Find(StatName))
	{
		return *Value;
	}
	
	return 0.f;
}

void ANPC::SetStat(FName StatName, float NewValue)
{
	if (float* Value = Stats.Find(StatName))
	{
		//*Value = NewValue;
		*Value = FMath::Clamp(NewValue, 0.f, 100.f);
	}
}

void ANPC::ModifyStat(FName StatName, float Delta)
{
	if (float* Value = Stats.Find(StatName))
	{
		*Value += Delta;
		*Value = FMath::Clamp(*Value, 0.f, 100.f);
	}
}

// Called every frame
void ANPC::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//RayCast();
	
	StatTimerRemaining -= DeltaTime;
	if (StatTimerRemaining <= 0.f)
	{
		UpdateStats();
		StatTimerRemaining = StatTimerInterval;
	}

	/*FVector ToTarget = GlobalDestination - GetActorLocation();

	FRotator CurrentRotation = GetActorRotation();
	FRotator TargetRotation = ToTarget.Rotation();
	float Angle = FMath::Abs(FMath::FindDeltaAngleDegrees(CurrentRotation.Yaw, TargetRotation.Yaw));
	float TurnSpeed = FMath::GetMappedRangeValueClamped(FVector2D(0.f, 180.f), FVector2D(8.f, 3.f), Angle);

	const FRotator NewRotation = FMath::RInterpTo(CurrentRotation, TargetRotation, DeltaTime, TurnSpeed);

	SetActorRotation(NewRotation);*/
}

// Called to bind functionality to input
void ANPC::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

