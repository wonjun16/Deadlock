// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Net/UnrealNetwork.h"
#include "Animation/AnimInstance.h"
#include "DeadLockAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class DEADLOCK_API UDeadLockAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:

	UFUNCTION(NetMulticast, Reliable)
	void MultiCastAnimation(bool bAnimIsRunning);

protected:

	virtual void NativeInitializeAnimation() override;

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = "Data")
	FVector Velocity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = "Data")
	float MoveSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = "Data")
	uint8 bIsMove : 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = "Data")
	uint8 bIsFalling : 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = "Data")
	float Direction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = "Data")
	uint8 bIsAnimZoom : 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = "Data")
	uint8 bIsAnimCrouch : 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = "Data")
	float PlayerAimPitch;

private:

	void OnRep_AnimIsRunning();

	void UpdateAnimationState();

};
