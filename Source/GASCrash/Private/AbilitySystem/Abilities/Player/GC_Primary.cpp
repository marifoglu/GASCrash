// Copyright ApesFiction


#include "AbilitySystem/Abilities/Player/GC_Primary.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "GameplayAbilityBlueprint.h"
#include "AbilitySystem/Abilities/Enemy/GC_HItReact.h"
#include "Engine/OverlapResult.h"
#include "GameplayTags/GCTags.h"



void UGC_Primary::SendHitReactEventToActor(const TArray<AActor*>& ActorsHit)
{
	for (AActor* HitActor : ActorsHit)
	{
		FGameplayEventData Payload;
		Payload.Instigator = GetAvatarActorFromActorInfo();
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(HitActor, GCTags::Events::Enemy::HitReact, Payload);
	}
}
