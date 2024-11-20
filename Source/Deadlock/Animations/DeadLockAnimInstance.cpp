// Fill out your copyright notice in the Description page of Project Settings.


#include "DeadLockAnimInstance.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Deadlock/Player/DeadlockCharacter.h"
#include "Kismet/KismetMathLibrary.h"
#include "KismetAnimationLibrary.h"

void UDeadLockAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	ACharacter* PlayerPawn = Cast<ACharacter>(TryGetPawnOwner());
	ADeadlockCharacter* Deadlockcharacter = Cast<ADeadlockCharacter>(TryGetPawnOwner());

	if (IsValid(PlayerPawn))
	{
		Velocity = PlayerPawn->GetCharacterMovement()->Velocity;
		MoveSpeed = PlayerPawn->GetCharacterMovement()->Velocity.Size2D();
		bIsMove = (UKismetMathLibrary::NotEqual_VectorVector(PlayerPawn->GetCharacterMovement()->GetCurrentAcceleration(), FVector::Zero(), 0.0f) && MoveSpeed > 3.0f);
		bIsFalling = PlayerPawn->GetCharacterMovement()->IsFalling();
		
		Direction = UKismetAnimationLibrary::CalculateDirection(Velocity, PlayerPawn->GetActorRotation());



	}

	if (IsValid(Deadlockcharacter))
	{
		bIsAnimZoom = Deadlockcharacter->bIsZoom;
		bIsAnimCrouch = Deadlockcharacter->bIsCrouch;
		PlayerAimPitch = Deadlockcharacter->LookAxisVector.Y;
	}
}