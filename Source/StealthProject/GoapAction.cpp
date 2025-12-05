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

void GoapAction::Start()
{
	Strategy->Start();
}

void GoapAction::Tick(float deltaTime)
{
	if(Strategy->CanPerform())
	{ 
		Strategy->Tick(deltaTime);
	}

	if (Strategy->Complete())
	{
		EvaluateEffects();
	}

	/*for (TSharedPtr<AgentBeliefs> effect : Effects)
	{
		effect->Evaluate();
	}*/
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


