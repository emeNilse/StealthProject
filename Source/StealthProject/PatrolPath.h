
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EdGraphSchema_K2.h"
#include "PatrolPath.generated.h"

//This script is used to create patrol paths in the world. When the AI is patrolling, it will find its nearest patrol path and patrol that route.

UCLASS()
class STEALTHPROJECT_API APatrolPath : public AActor
{
	GENERATED_BODY()
	
public:	
	APatrolPath();

	FVector GetPatrolPoint(int const index) const;

	int Num() const;

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI", meta=(MakeEditWidget="true", AllowPrivateAccess="true"))
	TArray<FVector> PatrolPoints;

};
