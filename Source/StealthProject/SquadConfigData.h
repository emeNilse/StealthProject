
#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "EnvironmentQuery/EnvQuery.h"
#include "EnvironmentQuery/EnvQueryManager.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "SquadConfigData.generated.h"


UCLASS(BlueprintType)
class STEALTHPROJECT_API USquadConfigData : public UDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere)
	UEnvQuery* AnchorQuery;

	UPROPERTY(EditAnywhere)
	UEnvQuery* UnitQuery;
};
