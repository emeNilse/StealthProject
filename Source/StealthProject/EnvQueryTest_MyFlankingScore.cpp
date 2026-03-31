
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
	float tarX = QueryInstance.NamedParams.FindRef(TEXT("TargetX"));
	float tarY = QueryInstance.NamedParams.FindRef(TEXT("TargetY"));
	float tarZ = QueryInstance.NamedParams.FindRef(TEXT("TargetZ"));
	FVector targetLoc(tarX, tarY, tarZ);

	
	float fX = QueryInstance.NamedParams.FindRef(TEXT("FlankDirX"));
	float fY = QueryInstance.NamedParams.FindRef(TEXT("FlankDirY"));
	float fZ = QueryInstance.NamedParams.FindRef(TEXT("FlankDirZ"));
	FVector flankDir(fX, fY, fZ);
	flankDir.Normalize();
	
	for (FEnvQueryInstance::ItemIterator It(this, QueryInstance); It; ++It)
	{
		FVector itemLoc = GetItemLocation(QueryInstance, It.GetIndex());
		
		float score = ScoreFlank(itemLoc, targetLoc, flankDir);
		
		It.SetScore(TestPurpose, FilterType, score, 0, 1);
	}
}
