
#include "BeliefFactory.h"



BeliefFactory::BeliefFactory(UGoapComponent* component, AgentBeliefs::BeliefRegistry& registry) : Component(component), Registry(registry)
{}

BeliefFactory::~BeliefFactory()
{
}

void BeliefFactory::AddBelief(FString key, TFunction<bool()> condition)
{
	TSharedPtr<AgentBeliefs> Belief = Registry.Get(key);

	AgentBeliefs::Builder(Belief).WithCondition(condition).Build();
	
	/*if (!Beliefs.Contains(key))
	{
		Beliefs.Add(key, AgentBeliefs::Builder(key).WithCondition(condition).Build());
	}*/
}


//void BeliefFactory::AddSensorBelief(FString key, USensor* sensor)
//{
//	
//}
//
//void BeliefFactory::AddLocationBelief(FString key, float distance, FVector& locationCondition)
//{
//	Beliefs.Add(key, AgentBeliefs::Builder(key).WithCondition([=, this]() { return InRangeOf(locationCondition, distance); }).WithLocation([=]() {return locationCondition;}).Build());
//}
//
//
//bool BeliefFactory::InRangeOf(FVector position, float range)
//{
//	AActor* owner = Component ? Component->GetOwner() : nullptr;
//
//	if (!owner)
//	{
//		return false;
//	}
//	
//	return FVector::Dist(owner->GetActorLocation(), position) <= range;
//}


