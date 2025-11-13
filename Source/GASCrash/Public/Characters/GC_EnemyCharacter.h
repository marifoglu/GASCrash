// Copyright ApesFiction

#pragma once

#include "CoreMinimal.h"
#include "GC_BaseCharacter.h"
#include "GC_EnemyCharacter.generated.h"

class UAbilitySystemComponent;

UCLASS()
class GASCRASH_API AGC_EnemyCharacter : public AGC_BaseCharacter
{
	GENERATED_BODY()

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

public:
	AGC_EnemyCharacter();

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(visibleAnywhere)
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;
};
