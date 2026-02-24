
#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Containers/Array.h"
#include "Containers/Set.h"
#include "IAction.h"
#include "ActionStack.generated.h"

//The mandatory Action Stack. 
//It's purpose was solely to meet the project requirements and run the game menus. 
//Based on Carl's Action Stack from Lecture 7, an exact replica translated from Unity C# into Unreal's C++, and is separate from the Action Stack Component made
// for Goap later on.

//The Action Stack was made as a Game Instance Subsystem, and not made a pure C++ script, so that it could be accessed in the editor and made Blueprint friendly.
//The Stack is originally created in UMyGameInstance, where Main Menu Widget gets pushed.
//After this, it is accessed from editor Blueprints to push new actions (more menu widgets).

//Did I have a fun time implementing this? No.
//Was it an educational experience? Headaches are normally associated with learning, so I guess so, yes.
//#hopefullyneveragain

UCLASS()
class STEALTHPROJECT_API UActionStack : public UGameInstanceSubsystem, public FTickableGameObject
{
	GENERATED_BODY()

public:
	~UActionStack();

	static UActionStack* Get(UWorld* World);

	virtual void Tick(float DeltaTime) override;
	virtual TStatId GetStatId() const override { RETURN_QUICK_DECLARE_CYCLE_STAT(UActionStack, STATGROUP_Tickables); }
	virtual bool IsTickable() const override { return true; }

	UFUNCTION(BlueprintCallable)
	void PushAction(UObject* Action);

	bool IsEmpty() const;

	UFUNCTION(BlueprintCallable)
	void ClearStack();

	const TArray<TObjectPtr<UObject>>& GetStack() const { return ActionStack; }
	TObjectPtr<UObject> GetCurrentAction() const { return CurrentAction; }

private:
	void UpdateActions();

	TArray<TObjectPtr<UObject>> ActionStack;

	TSet<TObjectPtr<UObject>> FirstTimeActions;

	TObjectPtr<UObject> CurrentAction;
};
