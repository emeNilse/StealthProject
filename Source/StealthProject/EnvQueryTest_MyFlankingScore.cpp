
#include "EnvQueryTest_MyFlankingScore.h"
#include "EnvironmentQuery/Items/EnvQueryItemType_VectorBase.h"
#include "Kismet/KismetMathLibrary.h"

UEnvQueryTest_MyFlankingScore::UEnvQueryTest_MyFlankingScore()
{
	//set this test to use vector locationsas its items
	ValidItemType = UEnvQueryItemType_VectorBase::StaticClass();
}

float UEnvQueryTest_MyFlankingScore::ScoreFlank(FVector& itemLocation, FVector& targetLocation, FVector& flankDirection) const
{
	FVector ToPoint = (itemLocation - targetLocation).GetSafeNormal();
	float Dot = FVector::DotProduct(ToPoint, flankDirection);
	
	return (Dot + 1.0f) * 0.5;
}

void UEnvQueryTest_MyFlankingScore::RunTest(FEnvQueryInstance& QueryInstance) const
{
	TArray<FVector> ContextLocations;
	QueryInstance.PrepareContext(TargetContext, ContextLocations);
	FVector TargetLocationFromContext = ContextLocations[0];
	
	TArray<AActor*> ContextActors;
	QueryInstance.PrepareContext(TargetContext, ContextActors);

	FlankDirectionX.BindData(QueryInstance.Owner.Get(), QueryInstance.QueryID);
	FlankDirectionY.BindData(QueryInstance.Owner.Get(), QueryInstance.QueryID);
	FlankDirectionZ.BindData(QueryInstance.Owner.Get(), QueryInstance.QueryID);

	float fX = FlankDirectionX.GetValue();
	float fY = FlankDirectionY.GetValue();
	float fZ = FlankDirectionZ.GetValue();
	FVector flankDir(fX, fY, fZ);
	flankDir.Normalize();
	
	for (FEnvQueryInstance::ItemIterator It(this, QueryInstance); It; ++It)
	{
		FVector itemLoc = GetItemLocation(QueryInstance, It.GetIndex());
		
		float score = ScoreFlank(itemLoc, TargetLocationFromContext, flankDir);
		
		It.SetScore(TestPurpose, FilterType, score, 0, 1);
	}
}
