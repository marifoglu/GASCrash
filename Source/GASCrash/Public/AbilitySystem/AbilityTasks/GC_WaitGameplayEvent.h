// Copyright ApesFiction

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Async/AbilityAsync_WaitGameplayEvent.h"
#include "GC_WaitGameplayEvent.generated.h"


UCLASS()
class GASCRASH_API UGC_WaitGameplayEvent : public UAbilityAsync_WaitGameplayEvent
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, Category = "Ability|Async", meta = (DefaultToSelf = "TargetActor", BlueprintInternalUseOnly = "TRUE"))
	static UGC_WaitGameplayEvent* WaitGameplayEventToActorProxy(AActor* TargetActor, UPARAM(meta=(GameplayTagFilter="GameplayEventTagsCategory")) FGameplayTag EventTag, bool OnlyTriggerOnce = false, bool OnlyMatchExact = true);

	void StartActivation();
};
