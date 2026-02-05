// Copyright ApesFiction


#include "UI/GC_AttributeWidget.h"

void UGC_AttributeWidget::OnAttributeChanged(const TTuple<FGameplayAttribute, FGameplayAttribute>& Pair,
	UGC_AttributeSet* AttributeSet, float OldValue)
{
	const float AttributeValue = Pair.Key.GetNumericValue(AttributeSet);
	const float MaxAttributeValue = Pair.Value.GetNumericValue(AttributeSet);

	BP_OnAttributeChange(AttributeValue, MaxAttributeValue, OldValue);
}

bool UGC_AttributeWidget::MatchesAttribute(const TTuple<FGameplayAttribute, FGameplayAttribute>& Pair) const
{
	return Attribute == Pair.Key && MaxAttribute == Pair.Value;
}
