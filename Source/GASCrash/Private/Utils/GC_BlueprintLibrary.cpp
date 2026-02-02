// Copyright ApesFiction



#include "Utils/GC_BlueprintLibrary.h"
#include "Characters/GC_BaseCharacter.h"
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

FClosestActorWithTagResult UGC_BlueprintLibrary::FindClosestActorWithTag(const UObject* WorldContextObject,
	const FVector& Origin, const FName Tag)
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
