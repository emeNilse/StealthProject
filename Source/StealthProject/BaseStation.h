
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/TextRenderComponent.h"
#include "Interactable.h"
#include "WorldFactProvider.h"
#include "GoapWorldStateComponent.h"
#include "BaseStation.generated.h"

//The Base Station is what every "station" the AI may interact with is inherited from. 
//Provides a text renderer to clearly display what station it is. 
//Has Interactable interface so the AI can Interact, if necessary, with the station.
//Stations also inherit from World Fact Provider, in case info about the station (e.g. storage of a resource) needs to
// be announced/shared with the AI.

UCLASS()
class STEALTHPROJECT_API ABaseStation : public AActor, public IInteractable, public IWorldFactProvider
{
	GENERATED_BODY()
	
public:	

	ABaseStation();

	virtual bool Interact_Implementation(AActor* interactor, EInteractionType type) override;

	virtual void GatherWorldFacts_Implementation(TArray<FWorldFact>& OutFacts) override;

protected:

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere)
	UTextRenderComponent* TextRenderer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString DisplayName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName StationTag;

private:
	void UpdateTextRotation();
};
