// Fill out your copyright notice in the Description page of Project Settings.


#include "GoapGoal.h"

GoapGoal::GoapGoal(FString name)
{
	Name = name;
}

GoapGoal::~GoapGoal()
{
}

GoapGoal::Builder::Builder(const FString name)
{
	goal->GoapGoal::GoapGoal(name);
}
