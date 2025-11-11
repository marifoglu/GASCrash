// Copyright ApesFiction

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/Character.h"
#include "GC_BaseCharacter.generated.h"

UCLASS(Abstract)
class GASCRASH_API AGC_BaseCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	AGC_BaseCharacter();
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;	
};
