// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameInstance.h"

UActionStack* UMyGameInstance::GetActionStack() const
{
	return GetSubsystem<UActionStack>();
}
