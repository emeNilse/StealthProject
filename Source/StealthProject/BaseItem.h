
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interactable.h"
#include "Components/TextRenderComponent.h"
#include "WorldFactProvider.h"
#include "InventoryComponent.h"
#include "BaseItem.generated.h"


UCLASS()
class STEALTHPROJECT_API ABaseItem : public AActor, public IInteractable, public IWorldFactProvider
{
	GENERATED_BODY()
	
public:	
	UPROPERTY(EditAnywhere)
	EItemType ItemType;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "World Facts")
	FName WorldFactKeyName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "World Facts")
	EWorldFactType FactType;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "World Facts")
	bool bIsItemActive = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "World Facts")
	bool bSingleOwner = false;

	bool bItemIsTaken = false;

	ABaseItem();

	virtual bool Interact_Implementation(AActor* interactor, EInteractionType type) override;

	virtual bool IsInteractionComplete_Implementation() override;

	virtual void GatherWorldFacts_Implementation(TArray<FWorldFact>& OutFacts) override;

	void SetOwner(TWeakObjectPtr<AActor> owner);

	void ItemTaken();

protected:
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere)
	UTextRenderComponent* TextRenderer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString DisplayName;

private:
	
	TWeakObjectPtr<AActor> Owner = nullptr;
	void UpdateTextRotation();

};
