// Copyright ApesFiction

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/PlayerState.h"
#include "GC_PlayerState.generated.h"


class UAbilitySystemComponent;
class UAttributeSet;

UCLASS()
class GASCRASH_API AGC_PlayerState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()
public:
	AGC_PlayerState();
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	UAttributeSet* GetAttributeSet() const { return AttributeSet;}
	
private:
	UPROPERTY(VisibleAnywhere, Category="Crash|Ability")
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet;
};
