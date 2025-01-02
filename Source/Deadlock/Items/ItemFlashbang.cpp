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

void AItemFlashbang::BeginPlay()
{
	Super::BeginPlay();

	if (FlashbangCurve)
	{
		FOnTimelineFloat UpdateTimeline;
		UpdateTimeline.BindUFunction(this, FName("UpdateBloomIntensityWeight"));

		FOnTimelineEvent FinishTimeline;
		FinishTimeline.BindUFunction(this, FName("FinishFlashbangEffect"));

		FlashbangTimeline.AddInterpFloat(FlashbangCurve, UpdateTimeline);
		FlashbangTimeline.SetTimelineFinishedFunc(FinishTimeline);
	}
}

void AItemFlashbang::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (FlashbangTimeline.IsPlaying())
	{
		FlashbangTimeline.TickTimeline(DeltaTime);
	}
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

				double FlashbangDotProduct = FVector::DotProduct(CharacterView, FlashbangDirection);

				//Check Character Looking Flash Point
				if (FlashbangDotProduct > 0.45f)
				{
					//Flash Effect
					UE_LOG(LogTemp, Log, TEXT("Character Is Looking Flash Point"));

					if (FlashbangCurve)
					{
						UE_LOG(LogTemp, Log, TEXT("TimelineStart"));
						AffectedCharacter(HitCharacter);
					}
				}
				else
				{
					UE_LOG(LogTemp, Log, TEXT("Character Is Not Looking Flash Point"));
				}
			}
		}
	}
	Super::ActivateAffect();
}

void AItemFlashbang::AffectedCharacter_Implementation(ADeadlockCharacter* HitCharacter)
{
	if (HitCharacter && HitCharacter->IsLocallyControlled())
	{
		AffectedCamera = HitCharacter->FollowCamera;
		if (AffectedCamera)
		{
			FlashbangTimeline.PlayFromStart();
		}
	}
}

void AItemFlashbang::UpdateBloomIntensityWeight(float Value)
{
	UE_LOG(LogTemp, Log, TEXT("Timeline Value: %f"), Value);
	if (AffectedCamera)
	{
		AffectedCamera->PostProcessSettings.bOverride_BloomIntensity = true;
		AffectedCamera->PostProcessSettings.BloomIntensity = Value * 300.0f;
		UE_LOG(LogTemp, Log, TEXT("PostProcessBlendWeight updated to %f on %s"), Value, *GetName());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("AffectedCamera is null on %s"), *GetName());
	}
}

void AItemFlashbang::FinishFlahbangEffect()
{
	if (AffectedCamera)
	{
		UE_LOG(LogTemp, Log, TEXT("FlashFinish"));
		AffectedCamera->PostProcessSettings.BloomIntensity = 0.675f;
		AffectedCamera->PostProcessSettings.bOverride_BloomIntensity = false;
	}
}