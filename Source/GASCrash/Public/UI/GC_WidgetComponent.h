// Copyright ApesFiction

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "Components/WidgetComponent.h"
#include "GC_WidgetComponent.generated.h"


class UAbilitySystemComponent;
class UAttributeSet;
class UGC_AttributeSet;
class UGC_AbilitySystemComponent;
class AGC_BaseCharacter;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class GASCRASH_API UGC_WidgetComponent : public UWidgetComponent
{
	GENERATED_BODY()
public:
	
protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
	TMap<FGameplayAttribute, FGameplayAttribute> AttributeMap;

private:
	TWeakObjectPtr<AGC_BaseCharacter> CrashCharacter;
	TWeakObjectPtr<UGC_AbilitySystemComponent> AbilitySystemComponent;
	TWeakObjectPtr<UGC_AttributeSet> AttributeSet;

	void InitAbilitySystemData();
	bool IsASCInitialized() const;
	void InitializedAttributeDelegate();
	void BindWidgetToAttributeChanges(UWidget* WidgetObject, const TTuple<FGameplayAttribute, FGameplayAttribute>& Pair) const;

	UFUNCTION()
	void BindToAttributeChanges();

	UFUNCTION()
	void OnASCInitialized(UAbilitySystemComponent* ASC, UAttributeSet* AS);
};
