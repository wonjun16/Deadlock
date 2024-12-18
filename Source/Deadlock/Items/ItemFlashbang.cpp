// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemFlashbang.h"
#include "Camera/CameraComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Deadlock/Player/DeadlockCharacter.h"

AItemFlashbang::AItemFlashbang()
{
	bReplicates = true;
	SetReplicateMovement(true);
	ItemTimer = 2.0f;
	EItemTypeIndex = 4;
}

void AItemFlashbang::ActivateAffect()
{
	TArray<FHitResult> HitActors;
	FVector FlashbangLocation = ItemMesh->GetComponentLocation();
	FVector Start = FlashbangLocation;
	FVector End = FlashbangLocation;

	FCollisionShape BlowRangeSphere = FCollisionShape::MakeSphere(500.0f);

	bool bIsHitInRange = GetWorld()->SweepMultiByChannel(HitActors, Start, End,
		FQuat::Identity, ECC_Pawn, BlowRangeSphere);


	if (bIsHitInRange)
	{
		for (auto& Hit : HitActors)
		{
			ADeadlockCharacter* HitCharacter = Cast<ADeadlockCharacter>(Hit.GetActor());

			if (HitCharacter)
			{
				FVector FlashbangDirection = (FlashbangLocation - HitCharacter->GetActorLocation()).GetSafeNormal();
				FVector CharacterView = HitCharacter->GetActorForwardVector().GetSafeNormal();
				UCameraComponent* CharacterCamera = HitCharacter->FollowCamera;

				double FlashbangDotProduct = FVector::DotProduct(CharacterView, FlashbangDirection);

				//Check Character Looking Flash Point
				if (FlashbangDotProduct > 0.45f)
				{
					//Flash Effect
					UE_LOG(LogTemp, Log, TEXT("Character Is Looking Flash Point"));

					//CharacterCamera->PostProcessBlendWeight = 200.0f;
					FlashEffect();
				}
				else
				{
					UE_LOG(LogTemp, Log, TEXT("Character Is Not Looking Flash Point"));
				}
			}
		}
	}
}
