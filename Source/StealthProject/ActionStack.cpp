// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionStack.h"

//UActionStack::UActionStack()
//{
//}

UActionStack::~UActionStack()
{
}

UActionStack* UActionStack::Get(UWorld* World)
{
	return World->GetGameInstance()->GetSubsystem<UActionStack>();
}

void UActionStack::Tick(float DeltaTime)
{
	UpdateActions();
}

void UActionStack::PushAction(TSharedPtr<IActionInterface> Action)
{
	if (!Action.IsValid())
	{
		return;
	}

	ActionStack.RemoveSingle(Action);
	ActionStack.Insert(Action, 0);

	if (CurrentAction.IsValid() && CurrentAction != Action)
	{
		CurrentAction.Reset();
	}
}

bool UActionStack::IsEmpty() const
{
	return !CurrentAction.IsValid() && ActionStack.Num() == 0;
}

void UActionStack::UpdateActions()
{
	if (IsEmpty())
	{
		return;
	}

	while (!CurrentAction.IsValid() && ActionStack.Num() > 0)
	{
		CurrentAction = ActionStack[0];

		bool bFirstTime = !FirstTimeSet.Contains(CurrentAction.Get());
		FirstTimeSet.Add(CurrentAction.Get());

		CurrentAction->OnBegin(bFirstTime);

		if (CurrentAction.IsValid())
		{
			//Did another action get pushed?
			if (ActionStack.Num() > 0 && CurrentAction != ActionStack[0])
			{
				CurrentAction.Reset();
				UpdateActions();
				return;
			}
		}

		if (CurrentAction.IsValid())
		{
			CurrentAction->OnUpdate();

			if (ActionStack.Num() > 0 && CurrentAction == ActionStack[0])
			{
				if (CurrentAction->IsDone())
				{
					ActionStack.RemoveAt(0);
					CurrentAction->OnEnd();
					FirstTimeSet.Remove(CurrentAction.Get());
					CurrentAction = nullptr;
				}
			}
			else
			{
				CurrentAction = nullptr;
			}
		}
	}

}
