// Copyright ApesFiction

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/GC_GameplayAbility.h"
#include "GC_Primary.generated.h"

/**
 * 
 */
UCLASS()
class GASCRASH_API UGC_Primary : public UGC_GameplayAbility
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category="Crash|Abilities")
	void SendHitReactEventToActor(const TArray<AActor*>& ActorsHit);

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Crash|Abilities")
	float HitBoxRadius = 100.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Crash|Abilities")
	float HitBoxForwardOffset = 200.0f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Crash|Abilities")
	float HitBoxElevationOffset = 20.0f;
};
