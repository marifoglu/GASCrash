// Copyright ApesFiction

#pragma once

#include "CoreMinimal.h"		
#include "Abilities/GC_GameplayAbility.h"
#include "GC_SearchForTarget.generated.h"


namespace EPathFollowingResult
{
	enum Type : int;
}

class UAITask_MoveTo;
class AGC_BaseCharacter;
class AGC_EnemyCharacter;
class UAbilityTask_WaitDelay;
class UGC_WaitGameplayEvent;
class AAIController;

UCLASS()
class GASCRASH_API UGC_SearchForTarget : public UGC_GameplayAbility
{
	GENERATED_BODY()

public:
	UGC_SearchForTarget();
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	
	TWeakObjectPtr<AGC_EnemyCharacter> OwningEnemy;
	TWeakObjectPtr<AAIController> OwningAIController;
	TWeakObjectPtr<AGC_BaseCharacter> TargetBaseCharacter;
	

private:

	UPROPERTY()
	TObjectPtr<UGC_WaitGameplayEvent> WaitGameplayEventTask;

	UPROPERTY()
	TObjectPtr<UAbilityTask_WaitDelay> SearchDelayTask;

	UPROPERTY()
	TObjectPtr<UAbilityTask_WaitDelay> AttackDelayTask;
	
	UPROPERTY()
	TObjectPtr<UAITask_MoveTo> MoveToLocationOrActorLocation;
	
	void StartSearch();

	UFUNCTION()
	void EndAttackEventReceived(FGameplayEventData Payload);

	UFUNCTION()
	void Search();
	
	void MoveToTargetAndAttack();

	UFUNCTION()
	void AttackTarget(TEnumAsByte<EPathFollowingResult::Type> Result, AAIController* AIController);

	UFUNCTION()
	void Attack();
};
