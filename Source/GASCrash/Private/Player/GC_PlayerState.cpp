// Copyright ApesFiction


#include "Player/GC_PlayerState.h"
#include "AbilitySystem/GC_AbilitySystemComponent.h"
#include "AbilitySystem/GC_AttributeSet.h"


AGC_PlayerState::AGC_PlayerState()
{
	SetNetUpdateFrequency(100.f);

	AbilitySystemComponent = CreateDefaultSubobject<UGC_AbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	AttributeSet = CreateDefaultSubobject<UGC_AttributeSet>("AttributeSet");
}

UAbilitySystemComponent* AGC_PlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}
