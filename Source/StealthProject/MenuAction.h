
#pragma once

#include "CoreMinimal.h"
#include "ActionBehaviour.h"
#include "MenuAction.generated.h"

//Menu Action inherits from the Action Behaviour, and simply creates a widget and set it to the View Port when it is implemented.
//The Menu Action behaviour is finished when a boolean bIsDone is set to true, which will trigger IsDone and remove itself from the stack. 
//It is a Blueprint class inherits from Menu Action, and keeps track of what widget is to be created. 
//The tricky part was trying to register the player controller to the widget so that the player can interact with the menu.

UCLASS()
class STEALTHPROJECT_API UMenuAction : public UActionBehaviour
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<UUserWidget> WidgetClass;

protected:
	UPROPERTY()
	TObjectPtr<UUserWidget> WidgetInstance;

	bool bIsDone = false;

public:
	virtual void OnBegin_Implementation(bool bFirstTime) override;
	virtual void OnEnd_Implementation() override;
	virtual bool IsDone_Implementation() const override;
	virtual void RequestDone() override;
};
