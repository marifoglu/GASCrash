// Copyright ApesFiction

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GC_Projectile.generated.h"

class UProjectileMovementComponent;
class UGameplayEffect;
UCLASS()
class GASCRASH_API AGC_Projectile : public AActor
{
	GENERATED_BODY()

public:
	AGC_Projectile();

	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(ExposeOnSpawn, ClampMin="0.0"), Category="Crash|Damage")
	float Damage{ -25.0f };

	UFUNCTION(BlueprintImplementableEvent, Category="Crash|Projectile")
	void SpawnImpactEffect();
private:
	UPROPERTY(VisibleAnywhere, Category="Crash|Projectile")
	TObjectPtr<UProjectileMovementComponent> ProjectileMovement;
	
	UPROPERTY(EditDefaultsOnly, Category="Crash|Damage")
	TSubclassOf<UGameplayEffect> DamageEffect;
};
