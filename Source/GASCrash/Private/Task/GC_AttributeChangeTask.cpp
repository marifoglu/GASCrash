// Copyright ApesFiction


#include "Task/GC_AttributeChangeTask.h"
#include "AbilitySystemComponent.h"

UGC_AttributeChangeTask* UGC_AttributeChangeTask::ListenForAttributeChange(
	UAbilitySystemComponent* AbilitySystemComponent, FGameplayAttribute Attribute)
{
	UGC_AttributeChangeTask* WaitForAttributeChangeTask = NewObject<UGC_AttributeChangeTask>();
	WaitForAttributeChangeTask->ASC = AbilitySystemComponent;
	WaitForAttributeChangeTask->AttributeToListenFor = Attribute;

	if (!IsValid(AbilitySystemComponent))
	{
		WaitForAttributeChangeTask->RemoveFromRoot();
		return nullptr;
	}

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(Attribute).AddUObject(WaitForAttributeChangeTask, &UGC_AttributeChangeTask::AttributeChanged);

	return WaitForAttributeChangeTask;
}

void UGC_AttributeChangeTask::EndTask()
{
	if (ASC.IsValid())
	{
		ASC->GetGameplayAttributeValueChangeDelegate(AttributeToListenFor).RemoveAll(this);
	}
	SetReadyToDestroy();
	MarkAsGarbage();
}

void UGC_AttributeChangeTask::AttributeChanged(const FOnAttributeChangeData& Data)
{
	OnAttributeChanged.Broadcast(Data.Attribute, Data.NewValue, Data.OldValue);
}
