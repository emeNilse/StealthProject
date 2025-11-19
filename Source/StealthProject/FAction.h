// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "IAction.h"
/**
 * 
 */
class STEALTHPROJECT_API FAction : public IActionInterface
{
public:
	FAction();
	~FAction();

	/*virtual void OnBegin(bool bFirstTime) override {}
	virtual void OnUpdate() override {}
	virtual void OnEnd() override {}
	virtual bool IsDone() override { return true; }

	virtual FString ToString() const override
	{
		return FString(TypeName());
	}

protected:
	const TCHAR* TypeName() const
	{
		return *FString(__FUNCTION__);
	}*/
};
