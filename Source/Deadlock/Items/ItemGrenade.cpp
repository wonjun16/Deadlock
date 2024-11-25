// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemGrenade.h"
#include "Kismet/GameplayStatics.h"
#include "Components/StaticMeshComponent.h"
#include "Deadlock/Player/DeadlockCharacter.h"

AItemGrenade::AItemGrenade()
{
	bReplicates = true;
	SetReplicateMovement(true);
	ItemTimer = 4.0f;
	EItemTypeIndex = 2;
}

void AItemGrenade::ClientItemAffect_Implementation()
{
	//Grenade Affect (Scan Character In Range, Give Damage)

	TArray<FHitResult> HitActors;
	FVector GrenadeLocation = ItemMesh->GetComponentLocation();
	FVector Start = GrenadeLocation;
	FVector End = GrenadeLocation;

	FCollisionShape HitRangeSphere = FCollisionShape::MakeSphere(300.0f);

	//Draw Two Diffrent Range Sphere
	bool bIsHitNearRange = GetWorld()->SweepMultiByChannel(HitActors, Start, End,
		FQuat::Identity, ECC_Pawn, HitRangeSphere);

	if (bIsHitNearRange)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Magenta, "Grenade Scan");
		for (auto& Hit : HitActors)
		{
			ADeadlockCharacter* HitCharacter = Cast<ADeadlockCharacter>(Hit.GetActor());
			if (HitCharacter)
			{
				FVector HitCharacterLocation = HitCharacter->GetActorLocation();
				FVector RangeofBurstPoint = GrenadeLocation - HitCharacterLocation;

				if (RangeofBurstPoint.Length() < 50.0f)
				{
					//Near Range Damage Event
					DamageAmount = 70.0f;
					UGameplayStatics::ApplyDamage(HitCharacter, DamageAmount,
						Owner->GetInstigatorController(), this, 0);
					GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Magenta, "Grenade Damage");
				}
				else
				{
					//Far Range Damage Event
					DamageAmount = 40.0f;
					UGameplayStatics::ApplyDamage(HitCharacter, DamageAmount,
						Owner->GetInstigatorController(), this, 0);
					GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Magenta, "Grenade Damage");
				}
			}
		}
		EndItemEvent();
	}
}
