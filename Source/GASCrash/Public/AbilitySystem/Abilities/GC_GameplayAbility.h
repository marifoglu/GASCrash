// Copyright ApesFiction

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "GC_GameplayAbility.generated.h"

/**
 * 
 */
UCLASS()
class GASCRASH_API UGC_GameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Crash|Debug")
	bool bDrawDebug = false;
};
