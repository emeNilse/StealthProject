// Fill out your copyright notice in the Description page of Project Settings.


#include "SquadManager.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

ASquadManager::ASquadManager()
{

}

void ASquadManager::FindMembers()
{
	TArray<TEnumAsByte<EObjectTypeQuery>> traceObjectTypes;
	traceObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn));

	TArray<AActor*> ignoreActors;
	ignoreActors.Init(this, 1);
	
	UKismetSystemLibrary::SphereOverlapActors(GetWorld(), GetActorLocation(), Radius, traceObjectTypes, ANPC::StaticClass(), ignoreActors, MySquad);
}

void ASquadManager::RoleAssignemnt()
{
}

void ASquadManager::BeginPlay()
{
	Super::BeginPlay();

	FindMembers();
}
