// Copyright ApesFiction


#include "UI/GC_WidgetComponent.h"
#include "AbilitySystem/GC_AbilitySystemComponent.h"
#include "AbilitySystem/GC_AttributeSet.h"
#include "Blueprint/WidgetTree.h"
#include "Characters/GC_BaseCharacter.h"
#include "UI/GC_AttributeWidget.h"


void UGC_WidgetComponent::BeginPlay()
{
	Super::BeginPlay();

	InitAbilitySystemData();

	if (!IsASCInitialized())
	{
		CrashCharacter->OnASCInitialized.AddDynamic(this, &ThisClass::OnASCInitialized);
		return;
	}
	InitializedAttributeDelegate();
}

void UGC_WidgetComponent::InitAbilitySystemData()
{
	CrashCharacter = Cast<AGC_BaseCharacter>(GetOwner());
	AttributeSet = Cast<UGC_AttributeSet>(CrashCharacter->GetAttributeSet());
	AbilitySystemComponent = Cast<UGC_AbilitySystemComponent>(CrashCharacter->GetAbilitySystemComponent());
}

bool UGC_WidgetComponent::IsASCInitialized() const
{
	return AbilitySystemComponent.IsValid() && AttributeSet.IsValid();
}

void UGC_WidgetComponent::InitializedAttributeDelegate()
{
	if (!AttributeSet->bAttributesInitialized)
	{
		AttributeSet->OnAttributesInitialized.AddDynamic(this, &ThisClass::BindToAttributeChanges);
	}
	else
	{
		BindToAttributeChanges();
	}
}

void UGC_WidgetComponent::BindToAttributeChanges()
{
	for (const TTuple<FGameplayAttribute, FGameplayAttribute>& Pair: AttributeMap)
	{
		BindWidgetToAttributeChanges(GetUserWidgetObject(), Pair); // for checking the owned widget object

		GetUserWidgetObject()->WidgetTree->ForEachWidget([this, &Pair](UWidget* ChildWidget)
		{
			BindWidgetToAttributeChanges(ChildWidget, Pair);
		});
	}
}

void UGC_WidgetComponent::OnASCInitialized(UAbilitySystemComponent* ASC, UAttributeSet* AS)
{
	AbilitySystemComponent = Cast<UGC_AbilitySystemComponent>(ASC);
	AttributeSet = Cast<UGC_AttributeSet>(AS);

	if (!IsASCInitialized()) return;
	
	InitializedAttributeDelegate();
}

void UGC_WidgetComponent::BindWidgetToAttributeChanges(UWidget* WidgetObject,
	const TTuple<FGameplayAttribute, FGameplayAttribute>& Pair) const
{
	UGC_AttributeWidget* AttributeWidget = Cast<UGC_AttributeWidget>(WidgetObject);
	if (!IsValid(AttributeWidget)) return; // We only care about CC Attribute Widgets
	if (!AttributeWidget->MatchesAttribute(Pair)) return; // Only subscribe for matching Attributes
	AttributeWidget->AvatarActor = CrashCharacter;
	

	AttributeWidget->OnAttributeChanged(Pair, AttributeSet.Get(), 0.f); // for initial values.

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(Pair.Key).AddLambda([this, AttributeWidget, &Pair](const FOnAttributeChangeData& AttributeChangeData)
	{
		AttributeWidget->OnAttributeChanged(Pair, AttributeSet.Get(), AttributeChangeData.OldValue); // For changes during the game.
	});
}

