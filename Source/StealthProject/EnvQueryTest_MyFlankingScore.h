
#pragma once

#include "CoreMinimal.h"
#include "EnvironmentQuery/EnvQueryTest.h"
#include "EnvQueryTest_MyFlankingScore.generated.h"


UCLASS()
class STEALTHPROJECT_API UEnvQueryTest_MyFlankingScore : public UEnvQueryTest
{
	GENERATED_BODY()

public: 
	UEnvQueryTest_MyFlankingScore();

	float ScoreFlank(FVector& itemLocation, FVector& targetLocation, FVector& flankDirection) const;

protected:
	virtual void RunTest(FEnvQueryInstance& QueryInstance) const override;

	UPROPERTY(EditDefaultsOnly, Category = "EQS Context")
	TSubclassOf<UEnvQueryContext> TargetContext;

	FAIDataProviderFloatValue FlankDirectionX;
	FAIDataProviderFloatValue FlankDirectionY;
	FAIDataProviderFloatValue FlankDirectionZ;
	
};
