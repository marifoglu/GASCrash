// Copyright ApesFiction


#include "AbilitySystem/AbilityTasks/GC_WaitGameplayEvent.h"

UGC_WaitGameplayEvent* UGC_WaitGameplayEvent::WaitGameplayEventToActorProxy(AActor* TargetActor,
	FGameplayTag EventTag, bool OnlyTriggerOnce, bool OnlyMatchExact)
{
	UGC_WaitGameplayEvent* MyObj = NewObject<UGC_WaitGameplayEvent>();
	MyObj->SetAbilityActor(TargetActor);
	MyObj->Tag = EventTag;
	MyObj->OnlyTriggerOnce = OnlyTriggerOnce;
	MyObj->OnlyMatchExact = OnlyMatchExact;
	return MyObj;
}

void UGC_WaitGameplayEvent::StartActivation()
{
	Activate();
}
