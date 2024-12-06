// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemFlashbang.h"
#include "Camera/CameraComponent.h"
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
	GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Red, "Flashbang Item Affect");

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
					UE_LOG(LogTemp, Log, TEXT("Character Is Looking Flash Point"));

					/*	CharacterCamera->PostProcessBlendWeight = 200.0f;

						if (FlashTimelineFloatCurve)
						{
							FlashTimeline->AddInterpFloat(FlashTimelineFloatCurve, UpdateFlashFloat);
						}*/
				}
				else
				{
					UE_LOG(LogTemp, Log, TEXT("Character Is Not Looking Flash Point"));
				}
			}
		}
	}
}
