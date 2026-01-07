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

void UActionStack::PushAction(UObject* Action)
{
	if (!Action || !Action->GetClass()->ImplementsInterface(UActionInterface::StaticClass()))
	{
		return;
	}

	//ActionStack.RemoveSingle(Action);
	ActionStack.Remove(Action);
	ActionStack.Insert(Action, 0);

	if (CurrentAction && CurrentAction != Action)
	{
		CurrentAction = nullptr;
	}
}

bool UActionStack::IsEmpty() const
{
	return !CurrentAction && ActionStack.Num() == 0;
}

void UActionStack::UpdateActions()
{
	if (IsEmpty())
	{
		return;
	}

	while (!CurrentAction && ActionStack.Num() > 0)
	{
		CurrentAction = ActionStack[0];

		//bool bFirstTime = !FirstTimeSet.Contains(CurrentAction.Get());
		bool bFirstTime = !FirstTimeActions.Contains(CurrentAction.Get());
		FirstTimeActions.Add(CurrentAction.Get());

		//CurrentAction->OnBegin(bFirstTime);
		IActionInterface::Execute_OnBegin(CurrentAction, bFirstTime);

		if (CurrentAction)
		{
			//Did another action get pushed?
			if (ActionStack.Num() > 0 && CurrentAction != ActionStack[0])
			{
				CurrentAction = nullptr;
				UpdateActions();
				return;
			}
		}

		if (CurrentAction)
		{
			//CurrentAction->OnUpdate();
			IActionInterface::Execute_OnUpdate(CurrentAction);

			if (ActionStack.Num() > 0 && CurrentAction == ActionStack[0])
			{
				if (IActionInterface::Execute_IsDone(CurrentAction))
				{
					ActionStack.RemoveAt(0);
					//CurrentAction->OnEnd();
					IActionInterface::Execute_OnEnd(CurrentAction);
					FirstTimeActions.Remove(CurrentAction.Get());
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
