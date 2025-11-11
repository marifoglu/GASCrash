// Copyright ApesFiction


#include "GASCrash/Public/Characters/GC_BaseCharacter.h"


// Sets default values
AGC_BaseCharacter::AGC_BaseCharacter()
{
	PrimaryActorTick.bCanEverTick = false;

	// Tick and refresh bone transforms whether rendered or not - for bone updates on a dedicated server
	GetMesh() -> VisibilityBasedAnimTickOption = EVisibilityBasedAnimTickOption::AlwaysTickPoseAndRefreshBones;
}

UAbilitySystemComponent* AGC_BaseCharacter::GetAbilitySystemComponent() const
{
	return nullptr;
}
