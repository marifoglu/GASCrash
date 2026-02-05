// Copyright ApesFiction


#include "GameObjects/GC_Projectile.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "Characters/GC_PlayerCharacter.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "GameplayTags/GCTags.h"
#include "Utils/GC_BlueprintLibrary.h"


AGC_Projectile::AGC_Projectile()
{
	PrimaryActorTick.bCanEverTick = false;

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileMovement");

	bReplicates = true;	
}

void AGC_Projectile::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	AGC_PlayerCharacter* PlayerCharacter = Cast<AGC_PlayerCharacter>(OtherActor);
	if (!IsValid(PlayerCharacter)) return;
	if (!PlayerCharacter->IsAlive()) return;

	UAbilitySystemComponent* AbilitySystemComponent = PlayerCharacter->GetAbilitySystemComponent();
	if (!IsValid(AbilitySystemComponent) || !HasAuthority()) return;

	FGameplayEventData Payload;
	Payload.Instigator = this;
	Payload.Target = PlayerCharacter;
	
	UGC_BlueprintLibrary::SendDamageEventToPlayer(PlayerCharacter, DamageEffect, Payload, GCTags::SetByCallers::Projectile, Damage, GCTags::None);
	
	SpawnImpactEffect();
	
	Destroy();
}

