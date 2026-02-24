
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GoapBelief.h"
#include "GoapBeliefSet.h"
#include "GoapAction.h"
#include "GoapActionsSet.h"
#include "GoapGoal.h"
#include "GoapGoalObject.h"
#include "CountdownTimer.h"
#include "ActionStackComponent.h"
#include "ActionPlan.h"
#include "GoapPlanner.h"
#include "AgentBeliefs.h"
#include "GoapActionStrategyBase.h"
#include "NPC.h"
#include "GoapPlannerInterface.h"
#include "GoapFactorySubsystem.h"
#include "GoapWorldStateComponent.h"
#include "WorldFactRegistry.h"
#include "BeliefFactory.h"
#include "GoapComponent.generated.h"

//The Goap Component is the heart of the operation, and definitely the most complex.
//While designing, and learning, about Goap, I was struggling to figure out which properties and functions actually belong here.
//For example, originally, this component took care of plan execution, that is until I decided to move all of that logic to the Action Stack Component.
//So this component is definitely open to improvements, and therefore I apologize for inefficiencies you may find in the structure.

//But basically, the AI gets this assigned this component, and from there a designer may add (in the editor) as many Goap Beliefs, Goap Actions, and Goap Goals to
//the respective TSets as they want.
//Upon construction, all beliefs, actions, and goals are registered in BeginPlay, and then the Component is ordered to set up a Action Plan.
//If the plan is null, complete, or failed, or is the AI (NPC class) has a state change, the component will set up a new plan. 

//Planning is done in the Goap Planner and the execution takes place in the Action Stack Component.

class AAI_Controller;

struct NPCLatestStats
{
	float Stamina;
	float OilAmountAtStation;
	int RefinedOreAtRefinery;
	bool bCanSeePlayer;
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class STEALTHPROJECT_API UGoapComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UGoapComponent();

	AAI_Controller* AI;
	ANPC* NPC;
	UBlackboardComponent* AI_BlackBoard;
	UActionStackComponent* ActionStackComponent;
	UGoapWorldStateComponent* WorldState;

	TSharedPtr<GoapAction> CurrentAction;

	TSet<TSharedPtr<GoapAction>> Actions;
	
	TSharedPtr<GoapGoal> CurrentGoal;

	TSet<TSharedPtr<GoapGoal>> Goals;

	TSharedPtr<ActionPlan> TheActionPlan;

	bool bShouldReplan = false;

	TUniquePtr<AgentBeliefs::BeliefRegistry> MyBeliefRegistry;

	TUniquePtr<IGoapPlannerInterface> GoapPlanner;
	UGoapFactorySubsystem* GoapFactory;

	TUniquePtr<BeliefFactory> MyBeliefFactory;

	NPCLatestStats LastNPCStats;
	ENPCState LastNPCState = ENPCState::Calm;

	UPROPERTY(EditAnywhere, Instanced, BlueprintReadWrite)
	TArray<UGoapBelief*> BeliefSet;

	UPROPERTY(EditAnywhere, Instanced, BlueprintReadWrite)
	TArray<UGoapActionsSet*> ActionSets;

	UPROPERTY(EditAnywhere, Instanced, BlueprintReadWrite)
	TArray<UGoapGoalObject*> GoalSet;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "TEXT")
	FString CurrentActionText;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "TEXT")
	FString CurrentGoalText;

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void MakeAPlanForActionStack();

	void HandlePlanFailed();

	void HandlePlanFinished();

	void SetupBeliefs();

	void SetupAction();

	void SetupGoals();

	void CalculatePlan();

	bool HasNPCStateChanged();

	UFUNCTION(BlueprintCallable)
	UGoapActionStrategyBase* GetCurrentStrategy() const { return CurrentAction->Strategy; }

	UFUNCTION(BlueprintCallable)
	FString GetCurrentGoal() const { return CurrentGoal->Name; }

private:

	TSharedPtr<GoapGoal> LastGoal;
};
