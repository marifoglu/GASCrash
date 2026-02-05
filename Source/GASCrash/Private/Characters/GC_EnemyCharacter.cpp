// Copyright ApesFiction


#include "Characters/GC_EnemyCharacter.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "Runtime/AIModule/Classes/AIController.h"
#include "AbilitySystem/GC_AbilitySystemComponent.h"
#include "AbilitySystem/GC_AttributeSet.h"
#include "GameplayTags/GCTags.h"
#include "Net/UnrealNetwork.h"



// Sets default values
AGC_EnemyCharacter::AGC_EnemyCharacter()
{
	PrimaryActorTick.bCanEverTick = false;

	AbilitySystemComponent = CreateDefaultSubobject<UGC_AbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);

	AttributeSet = CreateDefaultSubobject<UGC_AttributeSet>("AttributeSet");
}

void AGC_EnemyCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, bIsBeingLaunched);
}

UAbilitySystemComponent* AGC_EnemyCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

UAttributeSet* AGC_EnemyCharacter::GetAttributeSet() const
{
	return AttributeSet;
}

void AGC_EnemyCharacter::StopMovementUntilLanded()
{
	bIsBeingLaunched = true;
	AAIController* AIController = GetController<AAIController>();
	if (!IsValid(AIController)) return;

	AIController->StopMovement();

	if (!LandedDelegate.IsAlreadyBound(this, &ThisClass::EnableMobementOnLandeed))
	{
		LandedDelegate.AddDynamic(this, &ThisClass::EnableMobementOnLandeed);
	}
}

void AGC_EnemyCharacter::EnableMobementOnLandeed(const FHitResult& Hit)
{
	bIsBeingLaunched = false;
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(this, GCTags::Events::Enemy::EndAttack, FGameplayEventData());
	LandedDelegate.RemoveAll(this);
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



