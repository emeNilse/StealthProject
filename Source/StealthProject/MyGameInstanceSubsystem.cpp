
#include "MyGameInstanceSubsystem.h"

void UMyGameInstanceSubsystem::SetGlobalState(EGlobalState State)
{
	if (GlobalState == State)
	{
		return;
	}
	
	EGlobalState OldState = GlobalState;
	
	switch (State)
	{
	case EGlobalState::Init:
		GlobalState = State;
		break;
	case EGlobalState::Menu:
		GlobalState = State;
		break;
	case EGlobalState::Play:
		GlobalState = State;
		break;
	case EGlobalState::Pause:
		GlobalState = State;
		break;
	case EGlobalState::Outro:
		GlobalState = State;
		break;
	case EGlobalState::Quit:
		GlobalState = State;
		break;
	}

	OnGlobalStateChanged.Broadcast(OldState, GlobalState);
}
