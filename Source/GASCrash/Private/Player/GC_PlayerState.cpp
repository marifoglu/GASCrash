// Copyright ApesFiction


#include "Player/GC_PlayerState.h"
#include "AbilitySystemComponent.h"


AGC_PlayerState::AGC_PlayerState()
{
	SetNetUpdateFrequency(100.f);

	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);
}

UAbilitySystemComponent* AGC_PlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}
