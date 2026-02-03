// Fill out your copyright notice in the Description page of Project Settings.


#include "GoapAction.h"

GoapAction::GoapAction(FString name)
{
	Name = name;
	Cost = 1;
}

GoapAction::~GoapAction()
{
}

//telling the garbage collector to not touch Strategy
void GoapAction::AddReferencedObjects(FReferenceCollector& Collector)
{
	Collector.AddReferencedObject(Strategy);
}

FString GoapAction::GetReferencerName() const
{
	return TEXT("GoapAction");
}

void GoapAction::Start()
{
	Strategy->Start();
}

void GoapAction::Tick(float deltaTime)
{
	if(!Strategy->CanPerform())
	{ 
		/*Strategy->Tick(deltaTime);*/
	}

	if (Strategy->Status == EStrategyStatus::Running)
	{
		Strategy->Tick(deltaTime);
	}
}

bool GoapAction::IsDone()
{
	return Strategy->Complete();
}

EStrategyStatus GoapAction::StatusCheck()
{
	return Strategy->TickStatus();
}

void GoapAction::Stop()
{
	Strategy->Stop();
}

void GoapAction::EvaluateEffects()
{
	for (TSharedPtr<AgentBeliefs> effect : Effects)
	{
		effect->Evaluate();
	}
}

GoapAction::Builder::Builder(const FString name)
{
	action = MakeShared<GoapAction>(name);
}


