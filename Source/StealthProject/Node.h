// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GoapAction.h"


/**
 * 
 */
class STEALTHPROJECT_API Node
{
public:
	Node* Parent;

	TSharedPtr<GoapAction> Action;

	TSet<TSharedPtr<AgentBeliefs>> RequiredEffects;

	TArray<Node*> Leaves;

	float Cost = 0.f;

	bool IsLeafDead() const
	{
		return Leaves.Num() == 0 && Action == nullptr;
	}

	Node(Node* parent, TSharedPtr<GoapAction> action, TSet<TSharedPtr<AgentBeliefs>> effects, float cost);
};
