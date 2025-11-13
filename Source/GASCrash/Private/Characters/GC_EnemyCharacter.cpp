// Copyright ApesFiction


#include "Characters/GC_EnemyCharacter.h"

#include "AbilitySystemComponent.h"



// Sets default values
AGC_EnemyCharacter::AGC_EnemyCharacter()
{
	PrimaryActorTick.bCanEverTick = false;

	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);
}

UAbilitySystemComponent* AGC_EnemyCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
	return AbilitySystemComponent;
}
void AGC_EnemyCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (!IsValid(AbilitySystemComponent)) return;

	GetAbilitySystemComponent()->InitAbilityActorInfo(this,this);
	if (!HasAuthority()) return;

	GiveStartupAbilities();
}

