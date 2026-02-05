// Copyright ApesFiction


#include "Notifies/GC_MeleeAttack.h"
// #include "Animation/AnimInstanceProxy.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "KismetTraceUtils.h"
#include "Characters/GC_PlayerCharacter.h"
#include "GameplayTags/GCTags.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"

void UGC_MeleeAttack::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameeltaTime,
                                 const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyTick(MeshComp, Animation, FrameeltaTime, EventReference);

	if (!IsValid(MeshComp)) return;
	if (!IsValid(MeshComp->GetOwner())) return;

	TArray<FHitResult> Hits = PerformSphereTrace(MeshComp);
	SendEvents(MeshComp, Hits);
}

TArray<FHitResult> UGC_MeleeAttack::PerformSphereTrace(USkeletalMeshComponent* MeshComp) const
{
	TArray<FHitResult> OutHits;

	const FTransform SocketTransform = MeshComp->GetSocketTransform(SocketName);
	const FVector Start = SocketTransform.GetLocation(); // Socket Start Location
	const FVector ExtendedSocketDirection = UKismetMathLibrary::GetForwardVector(SocketTransform.GetRotation().Rotator()) * SocketExtensionOffset; // Socket Direction
	const FVector End = Start - ExtendedSocketDirection; // Socket End Location

	// TArray<AActor*> ActorsToIgnore;
	// ActorsToIgnore.Add(MeshComp->GetOwner());
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(MeshComp->GetOwner());

	FCollisionResponseParams ResponseParams;
	ResponseParams.CollisionResponse.SetAllChannels(ECR_Ignore);
	ResponseParams.CollisionResponse.SetResponse(ECC_Pawn, ECR_Block);
	
	UWorld* World = GEngine->GetWorldFromContextObject(MeshComp, EGetWorldErrorMode::LogAndReturnNull);
	if (!IsValid(World)) return OutHits;
	
	bool const bHit = World->SweepMultiByChannel(
		OutHits,
		Start,
		End,
		FQuat::Identity,
		ECC_Visibility,
		FCollisionShape::MakeSphere(SphereTraceRadius),
		Params,
		ResponseParams
		);

	if (bDrawDebugs)
	{
		DrawDebugSphereTraceMulti(
			World,
			Start,
			End,
			SphereTraceRadius,
			EDrawDebugTrace::ForDuration,
			bHit,
			OutHits,
			FColor::Red,
			FColor::Green,
			5.f
		);
	}
	return OutHits;
}

void UGC_MeleeAttack::SendEvents(USkeletalMeshComponent* MeshComp, const TArray<FHitResult>& Hits) const
{
	for (const FHitResult& Hit : Hits)
	{
		AGC_PlayerCharacter* PlayerCharacter = Cast<AGC_PlayerCharacter>(Hit.GetActor());
		if (!IsValid(PlayerCharacter)) continue;
		if (!PlayerCharacter->IsAlive()) continue;

		UAbilitySystemComponent* ASC = PlayerCharacter->GetAbilitySystemComponent();
		if (!IsValid(ASC)) continue;

		FGameplayEffectContextHandle ContextHandle = ASC->MakeEffectContext();
		ContextHandle.AddHitResult(Hit);

		FGameplayEventData Payload;
		Payload.Target = PlayerCharacter;
		Payload.ContextHandle = ContextHandle;
		Payload.Instigator = MeshComp->GetOwner();

		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(MeshComp->GetOwner(), GCTags::Events::Enemy::MeleeTraceHit, Payload);
	}
}
