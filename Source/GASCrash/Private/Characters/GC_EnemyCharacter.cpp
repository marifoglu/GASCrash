// Copyright ApesFiction


#include "Characters/GC_EnemyCharacter.h"
#include "AbilitySystem/GC_AbilitySystemComponent.h"
#include "AbilitySystem/GC_AttributeSet.h"
#include "Runtime/AIModule/Classes/AIController.h"
#include "UI/GC_AttributeWidget.h"


// Sets default values
AGC_EnemyCharacter::AGC_EnemyCharacter()
{
	PrimaryActorTick.bCanEverTick = false;

	AbilitySystemComponent = CreateDefaultSubobject<UGC_AbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);

	AttributeSet = CreateDefaultSubobject<UGC_AttributeSet>("AttributeSet");
}

UAbilitySystemComponent* AGC_EnemyCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
	return AbilitySystemComponent;
}

UAttributeSet* AGC_EnemyCharacter::GetAttributeSet() const
{
	return AttributeSet;
}

void AGC_EnemyCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (!IsValid(AbilitySystemComponent)) return;

	GetAbilitySystemComponent()->InitAbilityActorInfo(this,this);
	OnASCInitialized.Broadcast(GetAbilitySystemComponent(), GetAttributeSet());
	
	if (!HasAuthority()) return;

	GiveStartupAbilities();
	InitializeAttributes();

	UGC_AttributeSet* GC_AttributeSet = Cast<UGC_AttributeSet>(GetAttributeSet());
	if (!IsValid(GC_AttributeSet)) return;
	
	GetAbilitySystemComponent()->GetGameplayAttributeValueChangeDelegate(GC_AttributeSet->GetHealthAttribute()).AddUObject(this, &ThisClass::OnHealthChanged);
}

void AGC_EnemyCharacter::HandleDeath()
{
	Super::HandleDeath();

	AAIController* AIController = GetController<AAIController>();
	if (!IsValid(AIController)) return;

	AIController->StopMovement();
}

