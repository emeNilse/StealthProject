
#include "ActionBehaviour.h"

void UActionBehaviour::Initialize(APlayerController* InPlayerController)
{
	OwningPlayerController = InPlayerController;
}

void UActionBehaviour::RequestDone()
{
}
