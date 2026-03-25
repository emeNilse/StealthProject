
#include "NPC.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "GoapComponent.h"

ANPC::ANPC()
{
	PrimaryActorTick.bCanEverTick = true;

	WorldState = CreateDefaultSubobject<UGoapWorldStateComponent>(TEXT("WorldState"));

	VisionMesh = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("VisionMesh"));
	VisionMesh->SetupAttachment(GetCapsuleComponent());
	
	VisionMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	VisionMesh->SetGenerateOverlapEvents(false);

	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 300.f, 0.f);
}


void ANPC::BeginPlay()
{
	Super::BeginPlay();

	CachedWorld = GetWorld();

	StatTimerInterval = 1.f;
	StatTimerRemaining = 1.f;

	float HalfHeight = GetCapsuleComponent()->GetScaledCapsuleHalfHeight();

	VisionMesh->SetRelativeLocation(FVector(0, 0, -HalfHeight + 2.f));

	ConeMesh = Cast<UStaticMeshComponent>(GetComponentByClass(UStaticMeshComponent::StaticClass()));
	if (ConeMesh)
	{
		DynMaterialForScanner = ConeMesh->CreateAndSetMaterialInstanceDynamic(0);
		DynMaterialForScanner->SetVectorParameterValue("ConeColour", FLinearColor::Green);
	}
}

//RayCast was used for early debugging purposes, not sure if I might need it later. 
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

//For now, some of the NPC stats are simply updated here.
//The plan is to, eventually, move OilMeter and ResourceMeter to the actual stations.
void ANPC::UpdateStats()
{
	if (!bRecharging)
	{
		ModifyStat("Stamina", -5.f);
	}

	if (PlayerSpotted)
	{
		ModifyStat("Suspicious", 100);
	}
	else
	{
		ModifyStat("Suspicious", -5);
	}

	ModifyStat("OilMeter", 2);
	ModifyStat("ResourceMeter", 4);

	/*UE_LOG(LogTemp, Warning, TEXT("Stamina %f"), GetStat("Stamina"));*/
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

void ANPC::SetNPCState(ENPCState NewState)
{
	if (NPCState == NewState) return;

	if (NPCState == ENPCState::Alert)
	{
		CachedWorld->GetTimerManager().ClearTimer(AlertTimerhandle);
	}

	if (NPCState == ENPCState::Investigative)
	{
		CachedWorld->GetTimerManager().ClearTimer(InvestigativeTimerhandle);
	}
	
	LastNPCState = NPCState;

	NPCState = NewState;

	if (NPCState == ENPCState::Alert)
	{
		BeginAlert();
	}

	OnNPCStateChange();
}

void ANPC::ReturnToCalm()
{
	if (NPCState == ENPCState::Alert || NPCState == ENPCState::Investigative)
	{
		SetNPCState(ENPCState::Calm);
	}
}

void ANPC::ReturnToAlert()
{
	if (NPCState == ENPCState::Investigative)
	{
		SetNPCState(ENPCState::Alert);
	}
}

//When the AI becomes Alert, after 30 seconds it should return to a calm state.
void ANPC::BeginAlert()
{
	CachedWorld->GetTimerManager().SetTimer(AlertTimerhandle, this, &ANPC::ReturnToCalm, 30.f, false);
}

//When the AI becomes Investigative, if it gets stuck there, after 2 seconds it should return to a calm state.
//Originally I wanted something like this where the AI goes like "Hmmm..." before returning to being calm. 
//But apparently there's a bug somewhere that takes care of that already... and I haven't found it.
//Hence, this ensures the AI will return to normal.
void ANPC::BeginInvestigative()
{
	if (LastNPCState == ENPCState::Alert)
	{
		CachedWorld->GetTimerManager().SetTimer(InvestigativeTimerhandle, this, &ANPC::ReturnToAlert, 1.f, false);
	}
	else
	{
		CachedWorld->GetTimerManager().SetTimer(InvestigativeTimerhandle, this, &ANPC::ReturnToCalm, 1.f, false);
	}
}

void ANPC::OnNPCStateChange()
{
	switch (NPCState)
	{
	case ENPCState::Calm:
		//DynMaterial->SetVectorParameterValue("ConeColour", FLinearColor::Green);
		DynMaterialForScanner->SetVectorParameterValue("ConeColour", FLinearColor::Green);
		GetCharacterMovement()->MaxWalkSpeed = 300.f;
		PlayerSpotted = false;
		break;
	case ENPCState::Investigative:
		//DynMaterial->SetVectorParameterValue("ConeColour", FLinearColor::Blue);
		DynMaterialForScanner->SetVectorParameterValue("ConeColour", FLinearColor::Blue);
		GetCharacterMovement()->MaxWalkSpeed = 300.f;
		break;
	case ENPCState::Alert:
		//DynMaterial->SetVectorParameterValue("ConeColour", FLinearColor::Yellow);
		DynMaterialForScanner->SetVectorParameterValue("ConeColour", FLinearColor::Yellow);
		GetCharacterMovement()->MaxWalkSpeed = 400.f;
		PlayerSpotted = false;
		break;
	case ENPCState::Engaged:
		//DynMaterial->SetVectorParameterValue("ConeColour", FLinearColor::Red);
		DynMaterialForScanner->SetVectorParameterValue("ConeColour", FLinearColor::Red);
		GetCharacterMovement()->MaxWalkSpeed = 500.f;
		PlayerSpotted = true;
		break;
	}
}

//State debugging purposes. However, for now it looks good as a visual too.
void ANPC::InitializeVisionCone()
{
	if (AAI_Controller* AIController = Cast<AAI_Controller>(GetController()))
	{
		if (UAISenseConfig_Sight* Sight = AIController->GetSightConfig())
		{
			GenerateVisualCone(Sight->SightRadius, Sight->PeripheralVisionAngleDegrees, 32);
			/*float HalfAngle = 45.f;
			float Height = Sight->SightRadius;
			float Radius = Height * FMath::Tan(FMath::DegreesToRadians(HalfAngle));
			int32 Sides = 32;
			Generate3DVisual(Height, Radius, Sides);*/
		}
	}
}

void ANPC::GenerateVisualCone(float Radius, float HalfAngleDegrees, int32 NumSegments)
{
	TArray<FVector> Vertices;
	TArray<int32> Triangles;
	TArray<FVector> Normals;
	TArray<FVector2D> UVs;
	TArray<FProcMeshTangent> Tangents;

	float HalfAngleRad = FMath::DegreesToRadians(HalfAngleDegrees);

	Vertices.Add(FVector::ZeroVector);

	for (int32 i = 0; i <= NumSegments; i++)
	{
		float Alpha = (float)i / NumSegments;
		float Angle = FMath::Lerp(-HalfAngleRad, HalfAngleRad, Alpha);

		float X = Radius * FMath::Cos(Angle);
		float Y = Radius * FMath::Sin(Angle);

		Vertices.Add(FVector(X, Y, 0));
	}

	for (int32 i = 1; i <= NumSegments; i++)
	{
		Triangles.Add(0);
		Triangles.Add(i);
		Triangles.Add(i + 1);
	}

	for (int32 i = 0; i < Vertices.Num(); i++)
	{
		Normals.Add(FVector::UpVector);
		UVs.Add(FVector2D(0, 0));
		Tangents.Add(FProcMeshTangent(1, 0, 0));
	}

	VisionMesh->CreateMeshSection(0, Vertices, Triangles, Normals, UVs, TArray<FColor>(), Tangents, false);

	if (VisionMaterial)
	{
		VisionMesh->SetMaterial(0, VisionMaterial);
		DynMaterial = VisionMesh->CreateAndSetMaterialInstanceDynamic(0);
		DynMaterial->SetVectorParameterValue("ConeColour", FLinearColor::Green);
	}
}

//Attempt at a 3D "scanner" visualization, does not work well, but might attempt this again in the future.
void ANPC::Generate3DVisual(float Height, float Radius, int32 Sides)
{
	TArray<FVector> Vertices;
	TArray<int32> Triangles;
	TArray<FVector> Normals;
	TArray<FVector2D> UVs;
	TArray<FProcMeshTangent> Tangents;
	int ApexIndex = 0;
	Vertices.Add(FVector(Height, 0, 0));

	for (int32 i = 0; i < Sides; i++)
	{
		float AngleRad = 2 * PI * i / Sides;
		float Y = Radius * FMath::Cos(AngleRad);
		float Z = Radius * FMath::Sin(AngleRad);
		Vertices.Add(FVector(Height, Y, Z));
	}

	for (int32 i = 0; i < Sides; i++)
	{
		int32 Current = i + 1;
		int32 Next = (i + 1) % Sides + 1;

		Triangles.Add(ApexIndex);
		Triangles.Add(Current);
		Triangles.Add(Next);
	}

	int32 BaseCenterIndex = Vertices.Num();
	Vertices.Add(FVector(0, 0, 0));

	for (int32 i = 0; i < Sides; i++)
	{
		int32 Current = i + 1;
		int32 Next = (i + 1) % Sides + 1;

		Triangles.Add(BaseCenterIndex);
		Triangles.Add(Next);
		Triangles.Add(Current);
	}

	Normals.Init(FVector::UpVector, Vertices.Num());

	/*for (int32 i = 0; i < Triangles.Num(); i += 3)
	{
		FVector Edge1 = Vertices[Triangles[i + 1]] - Vertices[Triangles[i]];
		FVector Edge2 = Vertices[Triangles[i + 2]] - Vertices[Triangles[i]];
		FVector Normal = FVector::CrossProduct(Edge2, Edge1).GetSafeNormal();

		Normals[Triangles[i]] += Normal;
		Normals[Triangles[i + 1]] += Normal;
		Normals[Triangles[i + 2]] += Normal;
	}

	for (FVector& Normal : Normals)
	{
		Normal.Normalize();
	}*/

	for (int32 i = 0; i < Vertices.Num(); i++)
	{
		UVs.Add(FVector2D(0, 0));
		Tangents.Add(FProcMeshTangent(1, 0, 0));
	}

	VisionMesh->CreateMeshSection_LinearColor(0, Vertices, Triangles, Normals, UVs, TArray<FLinearColor>(), Tangents, false);

	if (VisionMaterial)
	{
		VisionMesh->SetMaterial(0, VisionMaterial);
		//VisionMesh->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));
	}
}

void ANPC::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	//InitializeVisionCone();
}

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
}

// Called to bind functionality to input
void ANPC::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

