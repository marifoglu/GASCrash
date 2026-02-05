// Copyright ApesFiction


#include "AbilitySystem/GC_SearchForTarget.h"
#include "AbilitySystemComponent.h"
#include "AIController.h"
#include "Abilities/Async/AbilityAsync_WaitGameplayEvent.h"
#include "Abilities/Tasks/AbilityTask_WaitDelay.h"
#include "AbilitySystem/AbilityTasks/GC_WaitGameplayEvent.h"
#include "Characters/GC_EnemyCharacter.h"
#include "GameplayTags/GCTags.h"
#include "Tasks/AITask_MoveTo.h"
#include "Utils/GC_BlueprintLibrary.h"

UGC_SearchForTarget::UGC_SearchForTarget()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::ServerOnly;
}

void UGC_SearchForTarget::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	OwningEnemy = Cast<AGC_EnemyCharacter>(GetAvatarActorFromActorInfo());
	check(OwningEnemy.IsValid());
	OwningAIController = Cast<AAIController>(OwningEnemy->GetController());
	check(OwningAIController.IsValid());

	StartSearch();

	WaitGameplayEventTask = UGC_WaitGameplayEvent::WaitGameplayEventToActorProxy(GetAvatarActorFromActorInfo(),GCTags::Events::Enemy::EndAttack);
	WaitGameplayEventTask->EventReceived.AddDynamic(this, &ThisClass::EndAttackEventReceived);
	WaitGameplayEventTask->StartActivation();
}

void UGC_SearchForTarget::StartSearch()
{
	if (!OwningEnemy.IsValid()) return;	

	const float SearchDelay = FMath::RandRange(OwningEnemy->MinAttackDelay, OwningEnemy->MaxAttackDelay);
	SearchDelayTask = UAbilityTask_WaitDelay::WaitDelay(this, SearchDelay);
	SearchDelayTask->OnFinish.AddDynamic(this, &ThisClass::Search);
	SearchDelayTask->Activate();
}

void UGC_SearchForTarget::EndAttackEventReceived(FGameplayEventData Payload)
{
	if (OwningEnemy.IsValid() && !OwningEnemy->bIsBeingLaunched)
	{
		StartSearch();
	}
}

void UGC_SearchForTarget::Search()
{
	const FVector SearchOrigin = GetAvatarActorFromActorInfo()->GetActorLocation();
	if (!OwningEnemy.IsValid()) return;
	FClosestActorWithTagResult ClosestActorResult = UGC_BlueprintLibrary::FindClosestActorWithTag(GetAvatarActorFromActorInfo(), SearchOrigin, CrashTags::Player, OwningEnemy->SearchRange);
	
	TargetBaseCharacter = Cast<AGC_BaseCharacter>(ClosestActorResult.Actor);
	if (!TargetBaseCharacter.IsValid())
	{
		StartSearch();
		return;
	}

	if (TargetBaseCharacter->IsAlive())
	{
		MoveToTargetAndAttack();
	}
	else
	{
		StartSearch();
	}
}

void UGC_SearchForTarget::MoveToTargetAndAttack()
{
	if (!OwningEnemy.IsValid() || !OwningAIController.IsValid() || !TargetBaseCharacter.IsValid()) return;
	if (!OwningEnemy->IsAlive())
	{
		StartSearch();
		return;
	}

	MoveToLocationOrActorLocation = UAITask_MoveTo::AIMoveTo(
		OwningAIController.Get(),
		FVector(),
		TargetBaseCharacter.Get(),
		OwningEnemy->AcceptanceRadius
		);

	MoveToLocationOrActorLocation->OnMoveTaskFinished.AddUObject(this, &ThisClass::AttackTarget);
	MoveToLocationOrActorLocation->ConditionalPerformMove();
	
}

void UGC_SearchForTarget::AttackTarget(TEnumAsByte<EPathFollowingResult::Type> Result, AAIController* AIController)
{
	if (Result != EPathFollowingResult::Success)
	{
		StartSearch();
		return;
	}

	OwningEnemy->RotateToTarget(TargetBaseCharacter.Get());
	
	AttackDelayTask = UAbilityTask_WaitDelay::WaitDelay(this, OwningEnemy->GetTimelineLength());
	AttackDelayTask->OnFinish.AddDynamic(this, &ThisClass::Attack);
	AttackDelayTask->Activate();
}

void UGC_SearchForTarget::Attack()
{
	const FGameplayTag AttackTag = GCTags::GCAbilities::Enemy::Attack;
	GetAbilitySystemComponentFromActorInfo()->TryActivateAbilitiesByTag(AttackTag.GetSingleTagContainer());
}