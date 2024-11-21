// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemFlashbang.h"
#include "Camera/CameraComponent.h"
#include "Deadlock/Player/DeadlockCharacter.h"

AItemFlashbang::AItemFlashbang()
{
	bReplicates = true;
	SetReplicateMovement(true);
	EItemTypeIndex = 4;
}

void AItemFlashbang::EventItemAffect_Implementation()
{
	//Player Camera Flash

	TArray<FHitResult> HitActors;
	FVector FlashbangLocation = ItemMesh->GetComponentLocation();
	FVector Start = FlashbangLocation;
	FVector End = FlashbangLocation;

	FCollisionShape BlowRangeSphere = FCollisionShape::MakeSphere(300.0f);

	bool bIsHitInRange = GetWorld()->SweepMultiByChannel(HitActors, Start, End,
		FQuat::Identity, ECC_Pawn, BlowRangeSphere);
	
	if (bIsHitInRange)
	{
		UE_LOG(LogTemp, Log, TEXT("Character Hit In Range"));
		for (auto& Hit : HitActors)
		{
			ADeadlockCharacter* HitCharacter = Cast<ADeadlockCharacter>(Hit.GetActor());

			if (HitCharacter)
			{
				FVector FlashbangDirection = (FlashbangLocation - HitCharacter->GetActorLocation()).GetSafeNormal();
				UCameraComponent* CharacterCamera = HitCharacter->FollowCamera;
				FVector FrontVector = CharacterCamera->GetForwardVector();
				FVector CharacterView = FrontVector.GetSafeNormal();

				double FlashbangDotProduct = FVector::DotProduct(CharacterView, FlashbangDirection);

				//Check Character Looking Flash Point
				if (FlashbangDotProduct > 0.5)
				{
					//Flash Effect
					UE_LOG(LogTemp, Log, TEXT("Character Looking Flash Point"));
				}
				else
				{
					UE_LOG(LogTemp, Log, TEXT("Character Not Looking Flash Point"));
				}
			}
		}
		EndItemEvent();
	}
}

void AItemFlashbang::StartItemTimer_Implementation()
{
	GetWorldTimerManager().SetTimer(ItemTriggerTimerHandle, this,
		&AItemBase::EventItemAffect, 0.4f, false, 4.0f);
}