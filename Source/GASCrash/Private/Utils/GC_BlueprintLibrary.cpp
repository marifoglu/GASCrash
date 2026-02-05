// Copyright ApesFiction



#include "Utils/GC_BlueprintLibrary.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "Characters/GC_BaseCharacter.h"
#include "Characters/GC_EnemyCharacter.h"
#include "Engine/OverlapResult.h"
#include "GameplayTags/GCTags.h"
#include "Kismet/GameplayStatics.h"

EHitDirection UGC_BlueprintLibrary::GetHitDirection(const FVector& TargetForward, const FVector& ToInstigator)
{
	const float DotProduct = FVector::DotProduct(TargetForward, ToInstigator);
	if (DotProduct < -0.5f)
	{
		return EHitDirection::Backward;
	}
	
	if (DotProduct < 0.5f)
	{
		// Either Left or Right
		const FVector Cross = FVector::CrossProduct(TargetForward, ToInstigator);
		if (Cross.Z < 0.f)
		{
			return EHitDirection::Left;
		}
		return EHitDirection::Right;
	}
	return EHitDirection::Forward;

}

FName UGC_BlueprintLibrary::GetHitDirectionName(const EHitDirection& HitDirection)
{
	switch (HitDirection)
	{
		case EHitDirection::Left : return FName("Left");
		case EHitDirection::Right : return FName("Right");
		case EHitDirection::Forward : return FName("Forward");
		case EHitDirection::Backward : return FName("Backward");
		default: return FName("None");
	}
}

FClosestActorWithTagResult UGC_BlueprintLibrary::FindClosestActorWithTag(UObject* WorldContextObject,
	const FVector& Origin, const FName Tag, float SearchRange)
{
	TArray<AActor*> ActorsWithTag;
	UGameplayStatics::GetAllActorsWithTag(WorldContextObject, Tag, ActorsWithTag);

	float ClosestDistance = TNumericLimits<float>::Max();
	AActor* ClosestActor = nullptr;

	for (AActor* Actor : ActorsWithTag)
	{
		if (!IsValid(Actor)) continue;
		AGC_BaseCharacter* BaseCharacter = Cast<AGC_BaseCharacter>(Actor);
		if (!IsValid(BaseCharacter) || !BaseCharacter->IsAlive()) continue;

		const float Distance = FVector::Dist(Origin, Actor->GetActorLocation());
		if (AGC_BaseCharacter* SearchingCharacter = Cast<AGC_BaseCharacter>(WorldContextObject); IsValid(SearchingCharacter))
		{
			if (Distance > SearchingCharacter->SearchRange) continue;
		}
		if (Distance < ClosestDistance)
		{
			ClosestDistance = Distance;
			ClosestActor = Actor;
		}
	}

	FClosestActorWithTagResult Result;
	Result.Actor = ClosestActor;
	Result.Distance = ClosestDistance;
	return Result;
}

void UGC_BlueprintLibrary::SendDamageEventToPlayer(AActor* Target, const TSubclassOf<UGameplayEffect>& DamageEffect,
	FGameplayEventData Payload, const FGameplayTag& DataTag, float Damage,const FGameplayTag& EventTagOverride, UObject* OptionalParticleSystem)
{
	AGC_BaseCharacter* PlayerCharacter = Cast<AGC_BaseCharacter>(Target);
	if (!IsValid(PlayerCharacter)) return;
	if (!PlayerCharacter->IsAlive()) return;

	FGameplayTag EventTag;
	if (!EventTagOverride.MatchesTagExact(GCTags::None))
	{
		EventTag = EventTagOverride;
	}else
	{
		UGC_AttributeSet* AttributeSet = Cast<UGC_AttributeSet>(PlayerCharacter->GetAttributeSet());
		if (!IsValid(AttributeSet)) return;

		const bool bLethal = AttributeSet->GetHealth() - Damage <= 0.f;
		EventTag= bLethal ? GCTags::Events::Player::Death : GCTags::Events::Player::HitReact;
	}
	


	Payload.OptionalObject = OptionalParticleSystem;
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(PlayerCharacter, EventTag, Payload);

	UAbilitySystemComponent* TargetASC = PlayerCharacter->GetAbilitySystemComponent();
	if (!IsValid(TargetASC)) return;

	FGameplayEffectContextHandle ContextHandle = TargetASC->MakeEffectContext();
	FGameplayEffectSpecHandle SpecHandle = TargetASC->MakeOutgoingSpec(DamageEffect, 1.f, ContextHandle);

	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, DataTag, -Damage);

	TargetASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());

}

void UGC_BlueprintLibrary::SendDamageEventToPlayers(TArray<AActor*> Target,
	const TSubclassOf<UGameplayEffect>& DamageEffect, FGameplayEventData Payload, const FGameplayTag& DataTag,
	float Damage, const FGameplayTag& EventTagOverride, UObject* OptionalParticleSystem)
{
	for (AActor* Target : Target)
	{
		SendDamageEventToPlayer(Target, DamageEffect, Payload, DataTag, Damage, EventTagOverride, OptionalParticleSystem);
	}
}

TArray<AActor*> UGC_BlueprintLibrary::HitBoxOverlapTest(AActor* AvatarActor, float HitBoxRadius, float HitBoxForwardOffset, float HitBoxElevationOffset, bool bDrawDebugs)
{
	if (!IsValid(AvatarActor)) return TArray<AActor*>();

	// Ensure that the overlap test ignores the Avatar Actor
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(AvatarActor);

	FCollisionResponseParams ResponseParams;
	ResponseParams.CollisionResponse.SetAllChannels(ECR_Ignore);
	ResponseParams.CollisionResponse.SetResponse(ECC_Pawn, ECR_Block);

	TArray<FOverlapResult> OverlapResults;
	FCollisionShape Sphere = FCollisionShape::MakeSphere(HitBoxRadius);

	const FVector Forward = AvatarActor->GetActorForwardVector() * HitBoxForwardOffset;
	const FVector HitBoxLocation = AvatarActor->GetActorLocation() + Forward + FVector(0.f, 0.f, HitBoxElevationOffset);

	UWorld* World = GEngine->GetWorldFromContextObject(AvatarActor, EGetWorldErrorMode::LogAndReturnNull);
	if (!IsValid(World)) return TArray<AActor*>();
	World->OverlapMultiByChannel(OverlapResults, HitBoxLocation, FQuat::Identity, ECC_Visibility, Sphere, QueryParams, ResponseParams);

	TArray<AActor*> ActorsHit;
	for (const FOverlapResult& Result : OverlapResults)
	{
		AGC_BaseCharacter* BaseCharacter = Cast<AGC_BaseCharacter>(Result.GetActor());
		if (!IsValid(BaseCharacter)) continue;
		if (!BaseCharacter->IsAlive()) continue;
		
		ActorsHit.AddUnique(BaseCharacter);
	}
	
	if (bDrawDebugs)
	{
		DrawHitBoxOverlapDebugs(AvatarActor,OverlapResults, HitBoxLocation, HitBoxRadius);
	}

	return ActorsHit;
}

void UGC_BlueprintLibrary::DrawHitBoxOverlapDebugs(const UObject* WorldContextObject, const TArray<FOverlapResult>& OverlapResults,
	const FVector& HitBoxLocation, float HitBoxRadius)
{
	UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull);
	if (!IsValid(World)) return;
	
	DrawDebugSphere(World, HitBoxLocation, HitBoxRadius, 16, FColor::Red, false, 3.f);

	for (const FOverlapResult& Result : OverlapResults)
	{
		if (IsValid(Result.GetActor()))
		{
			FVector DebugLocation = Result.GetActor()->GetActorLocation();
			DebugLocation.Z += 100.f;
			DrawDebugSphere(World, DebugLocation, 30.f, 10, FColor::Green, false, 3.f);
		}
	}
}

TArray<AActor*> UGC_BlueprintLibrary::ApplyKnockback(AActor* AvatarActor, const TArray<AActor*>& HitActors, float InnerRadius,
	float OuterForce, float LaunchForceMagnitude, float RotationAngle, bool bDrawDebugs)
{
	for (AActor* HitActor : HitActors)
	{
		ACharacter* HitCharacter = Cast<ACharacter>(HitActor);
		if (!IsValid(HitCharacter) || !IsValid(AvatarActor)) return TArray<AActor*>() ;
		
		const FVector HitCharacterLocation = HitCharacter->GetActorLocation();
		const FVector AvatarLocation = AvatarActor->GetActorLocation();
		
		const FVector ToHitActor = HitCharacterLocation - AvatarLocation;
		const float Distance = FVector::Dist(AvatarLocation, HitCharacterLocation);

		float LaunchForce = 0.f;
		if (Distance > OuterForce) continue;
		if (Distance <= InnerRadius)
		{
			LaunchForce = LaunchForceMagnitude;
		}
		else
		{
			const FVector2D FallofRange(InnerRadius, OuterForce);
			const FVector2D LaunchForceRange(LaunchForceMagnitude, 0.f);
		LaunchForce = FMath::GetMappedRangeValueClamped(FallofRange, LaunchForceRange, Distance);
		}

		if (bDrawDebugs) GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, FString::Printf(TEXT("Launch Force : %f"), LaunchForce));

		FVector KnockbackForce = ToHitActor.GetSafeNormal();
		KnockbackForce.Z = 0.f;

		const FVector Right = KnockbackForce.RotateAngleAxis(90.f, FVector::UpVector);
		KnockbackForce = KnockbackForce.RotateAngleAxis(-RotationAngle, Right) * LaunchForce;

		if (bDrawDebugs)
		{
			UWorld* World = GEngine->GetWorldFromContextObject(AvatarActor, EGetWorldErrorMode::LogAndReturnNull);
			DrawDebugDirectionalArrow(World, HitCharacterLocation, HitCharacterLocation + KnockbackForce, 100.f, FColor::Green, false, 3.f);
		}

		if (AGC_EnemyCharacter* EnemyCharacter = Cast<AGC_EnemyCharacter>(HitCharacter); IsValid(EnemyCharacter))
		{
			EnemyCharacter->StopMovementUntilLanded();
		}
		
		HitCharacter->LaunchCharacter(KnockbackForce, true, true);
	}
	return HitActors;
}
