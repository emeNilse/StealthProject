// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionStackComponent.h"

UActionStackComponent::UActionStackComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UActionStackComponent::PushAction(TSharedPtr<GoapAction> Action)
{
	ActionStack.Remove(Action);
	ActionStack.Insert(Action, 0);

	if (CurrentStackAction && CurrentStackAction != Action)
	{
		CurrentStackAction = nullptr;
	}
}

void UActionStackComponent::ClearStack()
{
	if (IsEmpty())
	{
		return;
	}

	ActionStack.Empty();
	FirstTimeActions.Empty();
	CurrentStackAction = nullptr;
}

bool UActionStackComponent::IsEmpty()
{
	return !CurrentStackAction && ActionStack.Num() == 0;
}

void UActionStackComponent::UpdateActions(float DeltaTime)
{
	if (IsEmpty())
	{
		
		return;
	}

	while (!CurrentStackAction && ActionStack.Num() > 0)
	{
		CurrentStackAction = ActionStack[0];
		bIsExecuting = false;
		bool bFirstTime = !FirstTimeActions.Contains(CurrentStackAction);

		//Shouldn't there be an if statement here with bFirstTime?
		FirstTimeActions.Add(CurrentStackAction);

		bool bAllPreconditionsMet = true;
		for (TSharedPtr<AgentBeliefs>& b : CurrentStackAction->Preconditions)
		{
			if (!b->Evaluate())
			{
				bAllPreconditionsMet = false;
				break;
			}
		}

		if (bAllPreconditionsMet)
		{
			CurrentStackAction->Start();
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("preconditions fail"));
			OnStackFailed.Broadcast();
			ClearStack();
		}

		if (CurrentStackAction)
		{
			//Did another action get pushed?
			if (ActionStack.Num() > 0 && CurrentStackAction != ActionStack[0])
			{
				CurrentStackAction = nullptr;
				UpdateActions(DeltaTime);
				return;
			}
		}
	}

	if (CurrentStackAction)
	{
		CurrentStackAction->Tick(DeltaTime);
		bIsExecuting = true;

		if (ActionStack.Num() > 0 && CurrentStackAction == ActionStack[0])
		{
			/*if (CurrentStackAction->IsDone())
			{
				ActionStack.RemoveAt(0);
				CurrentStackAction->Stop();
				CurrentStackAction->EvaluateEffects();
				FirstTimeActions.Remove(CurrentStackAction);
				CurrentStackAction = nullptr;

				if (IsEmpty())
				{
					UE_LOG(LogTemp, Warning, TEXT("stack is empty now"));
					OnStackFinished.Broadcast();
				}
			}*/

			if (CurrentStackAction->StatusCheck() == EStrategyStatus::Succeeded)
			{
				ActionStack.RemoveAt(0);
				CurrentStackAction->Stop();
				CurrentStackAction->EvaluateEffects();
				FirstTimeActions.Remove(CurrentStackAction);
				CurrentStackAction = nullptr;

				if (IsEmpty())
				{
					UE_LOG(LogTemp, Warning, TEXT("stack is empty now"));
					OnStackFinished.Broadcast();
				}
			}
			else if (CurrentStackAction->StatusCheck() == EStrategyStatus::Failed)
			{
				AbortCurrentAction();
			}

		}
		else
		{
			CurrentStackAction = nullptr;
		}
	}
}

void UActionStackComponent::AbortCurrentAction()
{
	if (CurrentStackAction.IsValid())
	{
		CurrentStackAction->Stop();
	}
	OnStackFailed.Broadcast();
	ClearStack();
}


void UActionStackComponent::BeginPlay()
{
	Super::BeginPlay();
}


void UActionStackComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	UpdateActions(DeltaTime);
}

