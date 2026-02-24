
#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "IAction.h"
#include "ActionBehaviour.generated.h"

//When I first started working on an Action Stack for this project, I wanted to only use it for Menu and UI logic. 
//The design is based on Carl's lecture 7 where he showed how to control the menus via the action stack.
//To make it work, as you will see, Menu Action inherits from the Action Behaviour, and simply creates a widget and set it to the View Port when it is implemented.
//The Menu Action behaviour is finished when a boolean is set to true, which will trigger IsDone and remove itself from the stack. 
// It is a Blueprint class inherits from Menu Action, and keeps track of what widget is to be created. 

//Any parts that implement the Action Stack inherit from IAction, which I called IActionInterface (had a hard time remembering Unreal naming 
// standards at the time, and I did not want to go through the Unreal brain-surgery-complexity of renaming scripts, 
// which I have seen give Unreal fatal heart attacks at times).

//Wrapping my head around how the action stack worked in the Unity project was one headache, trying to translate that into C++ and
//make it Blueprint friendly was a nightmare. 

UCLASS(Blueprintable, EditInlineNew)
class STEALTHPROJECT_API UActionBehaviour : public UObject, public IActionInterface
{
	GENERATED_BODY()

public:
	
	UFUNCTION(BlueprintCallable)
	void Initialize(APlayerController* InPlayerController);

	virtual void OnBegin_Implementation(bool bFirstTime) override {}

	virtual void OnUpdate_Implementation() override {}

	virtual void OnEnd_Implementation() override {}

	virtual bool IsDone_Implementation() const override { return true; }

	UFUNCTION(BlueprintCallable)
	virtual void RequestDone();

protected:
	UPROPERTY()
	APlayerController* OwningPlayerController;
};
