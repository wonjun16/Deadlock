// Fill out your copyright notice in the Description page of Project Settings.


#include "DeadLockAnimInstance.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Deadlock/Player/DeadlockCharacter.h"
#include "Deadlock/GameMode/DeadlockPlayerState.h"
#include "Kismet/KismetMathLibrary.h"
#include "KismetAnimationLibrary.h"

void UDeadLockAnimInstance::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UDeadLockAnimInstance, Velocity);
	DOREPLIFETIME(UDeadLockAnimInstance, MoveSpeed);
	DOREPLIFETIME(UDeadLockAnimInstance, bIsMove);
	DOREPLIFETIME(UDeadLockAnimInstance, bIsFalling);
	DOREPLIFETIME(UDeadLockAnimInstance, Direction);
	DOREPLIFETIME(UDeadLockAnimInstance, bIsAnimZoom);
	DOREPLIFETIME(UDeadLockAnimInstance, bIsAnimCrouch);
	DOREPLIFETIME(UDeadLockAnimInstance, bIsAnimThrow);
	DOREPLIFETIME(UDeadLockAnimInstance, bIsAnimHeal);
	DOREPLIFETIME(UDeadLockAnimInstance, PlayerAimPitch);
}

void UDeadLockAnimInstance::NativeInitializeAnimation()
{
}

void UDeadLockAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	ACharacter* PlayerPawn = Cast<ACharacter>(TryGetPawnOwner());
	ADeadlockCharacter* Deadlockcharacter = Cast<ADeadlockCharacter>(TryGetPawnOwner());

	if (IsValid(PlayerPawn))
	{
		Velocity = PlayerPawn->GetCharacterMovement()->Velocity;
		MoveSpeed = PlayerPawn->GetCharacterMovement()->Velocity.Size2D();
		bIsMove = (UKismetMathLibrary::NotEqual_VectorVector(PlayerPawn->GetCharacterMovement()->GetCurrentAcceleration(),
			FVector::Zero(), 0.0f) && MoveSpeed > 3.0f);
		bIsFalling = PlayerPawn->GetCharacterMovement()->IsFalling();
		
		Direction = UKismetAnimationLibrary::CalculateDirection(Velocity, PlayerPawn->GetActorRotation());

		PlayerAimPitch = PlayerPawn->GetBaseAimRotation().Pitch;
	}

	if (IsValid(Deadlockcharacter))
	{
		bIsAnimZoom = Deadlockcharacter->bIsZoom;
		bIsAnimCrouch = Deadlockcharacter->bIsCrouch;
		bIsAnimThrow = Deadlockcharacter->bIsThrow;
		bIsAnimHeal = Deadlockcharacter->bIsHeal;		
	}
}

void UDeadLockAnimInstance::OnRep_AnimIsRunning()
{
}

void UDeadLockAnimInstance::UpdateAnimationState()
{
}
