// Copyright ApesFiction

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystem/GC_AttributeSet.h"
#include "GameFramework/Character.h"
#include "GC_BaseCharacter.generated.h"

namespace CrashTags
{
	extern GASCRASH_API const FName Player;
}

class UAttributeSet;
class UGameplayEffect;
class UGameplayAbility;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FASCInitialized, UAbilitySystemComponent*, ASC, UAttributeSet*, AS);

UCLASS(Abstract)
class GASCRASH_API AGC_BaseCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	AGC_BaseCharacter();
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	virtual UAttributeSet* GetAttributeSet() const { return nullptr; }
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	bool IsAlive() const { return bAlive; }
	void SetAlive(bool bAliveStatus) { bAlive = bAliveStatus; }

	UPROPERTY(BlueprintAssignable)
	FASCInitialized OnASCInitialized;

	UFUNCTION(BlueprintCallable, Category="Crash|Death")
	virtual void HandleRespawn();

	UFUNCTION(BlueprintCallable, Category="Crash|Attributes")
	void ResetAttributes();

	UFUNCTION(BlueprintImplementableEvent)
	void RotateToTarget(AActor* RotateTarget);
	
protected:
	void GiveStartupAbilities();	
	void InitializeAttributes() const;

	void OnHealthChanged(const FOnAttributeChangeData& AttributeChangeData);
	virtual void HandleDeath();
private:
	UPROPERTY(EditDefaultsOnly, Category="Crash|Abilities");
	TArray<TSubclassOf<UGameplayAbility>> StartupAbilities;

	UPROPERTY(EditDefaultsOnly, Category="Crash|Effects");
	TSubclassOf<UGameplayEffect> InitializeAttributesEffect;
	
	UPROPERTY(EditDefaultsOnly, Category="Crash|Effects");
	TSubclassOf<UGameplayEffect> ResetAttributesEffect;
	

	UPROPERTY(BlueprintReadOnly, meta=(AllowPrivateAccess="true"),Replicated)
	bool bAlive = true;
};
