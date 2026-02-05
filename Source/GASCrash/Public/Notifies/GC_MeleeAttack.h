// Copyright ApesFiction

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "GC_MeleeAttack.generated.h"


UCLASS()
class GASCRASH_API UGC_MeleeAttack : public UAnimNotifyState
{
	GENERATED_BODY()

public:
	virtual void NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameeltaTime, const FAnimNotifyEventReference& EventReference) override;
	
private:
	UPROPERTY(EditAnywhere, Category="Crash|Debugs")
	bool bDrawDebugs = true;
	
	UPROPERTY(EditAnywhere, Category="Crash|Socket")
	FName SocketName{"FX_Trail_01_R"};

	UPROPERTY(EditAnywhere, Category="Crash|Socket")
	float SocketExtensionOffset{40.f};
	
	UPROPERTY(EditAnywhere, Category="Crash|Socket")
	float SphereTraceRadius{60.f};

	TArray<FHitResult> PerformSphereTrace(USkeletalMeshComponent* MeshComp) const;

	void SendEvents(USkeletalMeshComponent* MeshComp, const TArray<FHitResult>& Hits) const;
};
