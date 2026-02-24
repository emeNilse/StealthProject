
#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "ActionStack.h"
#include "MyGameInstance.generated.h"

//To push the Main Menu into the Action Stack, I had to hard code it find the Blueprint that would create the Main Menu Widget and
//push it into the Action Stack on start-up.
//How to do so was not straight forward...
//The tricky part was trying to register the player controller to the widget so that the player can interact with the menu.

class UActionBehaviour;

UCLASS()
class STEALTHPROJECT_API UMyGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	UMyGameInstance();

	virtual void Init() override;

	UFUNCTION(BlueprintCallable, Category = "Subsystem")
	UActionStack* GetActionStack() const;

protected:
	UPROPERTY(EditDefaultsOnly, Category="Startup")
	TSubclassOf<UActionBehaviour> StartupAction;

	UPROPERTY()
	UActionStack* Stack;

private:
	void HandlePostLoadMap(UWorld* loadedLevel);

	bool bStartupActionPushed = false;

	void TryPushStartupAction(UWorld* World, ELevelTick TickType, float DeltaSeconds);

};
