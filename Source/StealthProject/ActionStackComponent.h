
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GoapAction.h"
#include "ActionStackComponent.generated.h"

//The Action Stack Component was made sometime after completing the Action Stack that ran the menu and completing the Goap Component.
//It was when I noticed how the Goap Planner laid out its plan, and how the Goap Component was rummaging through the plan (executing, stopping, running next action),
//that I realized that I had almost made a exact copy of Carl's Action Stack design and that a Goap Plan runs similar to an action stack.
//So, I created the Action Stack Component to clean the Goap Component and separate Goap execution from the planning. 
//Not as streamlined as the Action Stack for running an entire game, but it controls the AI.

DECLARE_MULTICAST_DELEGATE(FOnActionStackFailed);
DECLARE_MULTICAST_DELEGATE(FOnActionStackFinished);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class STEALTHPROJECT_API UActionStackComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UActionStackComponent();

	FOnActionStackFailed OnStackFailed;
	FOnActionStackFinished OnStackFinished;

	UPROPERTY()
	AAI_Controller* OwnerAI;

	void PushAction(TSharedPtr<GoapAction> Action);

	void ClearStack();

	bool IsEmpty();

	void UpdateActions(float DeltaTime);

	void AbortCurrentAction();

	const TArray<TSharedPtr<GoapAction>>& GetStack() const { return ActionStack; }

	TSharedPtr<GoapAction> GetCurrentAction() const { return CurrentStackAction; }

	bool IsExecuting() const { return bIsExecuting; }

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:

	TArray<TSharedPtr<GoapAction>> ActionStack;
	TSet<TSharedPtr<GoapAction>> FirstTimeActions;
	TSharedPtr<GoapAction> CurrentStackAction;
	bool bIsExecuting;
};
