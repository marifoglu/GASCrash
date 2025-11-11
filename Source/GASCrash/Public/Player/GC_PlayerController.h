// Copyright ApesFiction

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GC_PlayerController.generated.h"

class UInputMappingContext;
class UInputAction;
struct FInputActionValue;

UCLASS()
class GASCRASH_API AGC_PlayerController : public APlayerController
{
	GENERATED_BODY()

protected:
	virtual void SetupInputComponent() override;

private:
	UPROPERTY(EditDefaultsOnly, Category="Crash|Input")
	TArray<TObjectPtr<UInputMappingContext>> InputMappingContexts;
	
	UPROPERTY(EditDefaultsOnly, Category="Crash|Input|Movement")
	TObjectPtr<UInputAction> JumpAction;
	UPROPERTY(EditDefaultsOnly, Category="Crash|Input|Movement")
	TObjectPtr<UInputAction> LookAction;
	UPROPERTY(EditDefaultsOnly, Category="Crash|Input|Movement")
	TObjectPtr<UInputAction> MoveAction;

	UPROPERTY(EditDefaultsOnly, Category="Crash|Input|Abilities")
	TObjectPtr<UInputAction> PrimaryAction;

	void Jump();
	void StopJumping();
	void Move(const FInputActionValue& InputValue);
	void Look(const FInputActionValue& InputValue);
	
	void Primary();
	
};
