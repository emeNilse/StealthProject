// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
//#include "UObject/Interface.h"
//#include "GoapPlannerInterface.generated.h"


class ActionPlan;
class UGoapComponent;
class GoapGoal;
/**
 * 
 */
//UINTERFACE(MinimalAPI)
//class UGoapPlannerInterface : public UInterface
//{
//	GENERATED_BODY()
//	
//};

class IGoapPlannerInterface
{
public:
	//virtual ~IGoapPlannerInterface();
	virtual ActionPlan* Plan(UGoapComponent* agent, TSet<TSharedPtr<GoapGoal>> goals, TSharedPtr<GoapGoal> mostRecentGoal = nullptr) = 0;

};
