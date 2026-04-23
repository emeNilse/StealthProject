
#include "BaseItem.h"
#include "WorldFactRegistry.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"
#include "Camera/PlayerCameraManager.h"

ABaseItem::ABaseItem()
{
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	TextRenderer = CreateDefaultSubobject<UTextRenderComponent>(TEXT("TextRenderer"));
	TextRenderer->SetupAttachment(RootComponent);

	DisplayName = TEXT("Item name");
}

bool ABaseItem::Interact_Implementation(AActor* interactor, EInteractionType type)
{
	if (!interactor) return false;

	if (type != EInteractionType::Default && type != EInteractionType::Item_PickUp) return false;

	if (UInventoryComponent* inventory = interactor->FindComponentByClass<UInventoryComponent>())
	{
		if (inventory->PickUpItem(ItemType))
		{
			Destroy();
			return true;
		}
	}
	return false;
}

bool ABaseItem::IsInteractionComplete_Implementation()
{
	return true;
}

void ABaseItem::GatherWorldFacts_Implementation(TArray<FWorldFact>& OutFacts)
{
	FWorldFact Fact;
	Fact.Key = WorldFactKeyName;
	Fact.BaseType = EWorldBaseType::Item;
	Fact.InfoType = FactType;
	Fact.BoolValue = bIsItemActive;
	Fact.IntValue = 1;
	Fact.Location = GetActorLocation();
	Fact.Source = TWeakObjectPtr<AActor>(this);
	Fact.SingleOwner = bSingleOwner;
	Fact.IsAlreadyTaken = bItemIsTaken;
	Fact.OwningActor = Owner;

	OutFacts.Add(Fact);
}

void ABaseItem::SetOwner(TWeakObjectPtr<AActor> owner)
{
	Owner = owner;
	ItemTaken();
}

void ABaseItem::ItemTaken()
{
	if (bItemIsTaken) return;

	bItemIsTaken = true;	
}

void ABaseItem::BeginPlay()
{
	Super::BeginPlay();
	TextRenderer->SetText(FText::FromString(DisplayName));
	TextRenderer->SetRelativeLocation(FVector(0, 0, 100));
	TextRenderer->SetWorldSize(40);

	if (UGameInstance* GI = GetGameInstance())
	{
		if (UWorldFactRegistry* Registry = GI->GetSubsystem<UWorldFactRegistry>())
		{
			Registry->Register(this);
		}
	}
}

void ABaseItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	UpdateTextRotation();
}

void ABaseItem::UpdateTextRotation()
{
	if (!TextRenderer) return;

	APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0);
	if (!PC) return;

	FVector CameraLocation = PC->PlayerCameraManager->GetCameraLocation();

	FVector TextLocation = TextRenderer->GetComponentLocation();

	TextRenderer->SetWorldRotation((CameraLocation - TextLocation).Rotation());
}
