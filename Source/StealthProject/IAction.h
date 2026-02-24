
#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "IAction.generated.h"

//Action Interface used for the Action Stack.
//The design is based on Carl's lecture 7 where he showed how to control the menus via the action stack.

UINTERFACE(Blueprintable)
class STEALTHPROJECT_API UActionInterface : public UInterface
{
	GENERATED_BODY()
};


class STEALTHPROJECT_API IActionInterface
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintNativeEvent)
	void OnBegin(bool bFirstTime);

	UFUNCTION(BlueprintNativeEvent)
	void OnUpdate();

	UFUNCTION(BlueprintNativeEvent)
	void OnEnd();

	UFUNCTION(BlueprintNativeEvent)
	bool IsDone() const;

};
