// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemGrenade.h"
#include "Deadlock/Player/DeadlockCharacter.h"

void AItemGrenade::UseItem_Implementation(int currentitemcount)
{
	if (currentitemcount >= 0)
	{
		
	}
}

void AItemGrenade::EventItemAffect_Implementation()
{
	//Grenade Affect (Scan Character In Range, Give Damage)

	TArray<FHitResult> HitActors;
	FVector GrenadeLocation = GetActorLocation();
	FVector Start = GrenadeLocation;
	FVector End = GrenadeLocation;

	FCollisionShape HitRangeSphere = FCollisionShape::MakeSphere(300.0f);

	//Draw Two Diffrent Range Sphere
	bool bIsHitNearRange = GetWorld()->SweepMultiByChannel(HitActors, Start, End,
		FQuat::Identity, ECC_Pawn, HitRangeSphere);

	if (bIsHitNearRange)
	{
		for (auto& Hit : HitActors)
		{
			ADeadlockCharacter* HitCharacter = Cast<ADeadlockCharacter>(Hit.GetActor());
			if (HitCharacter)
			{
				FVector HitCharacterLocation = HitCharacter->GetActorLocation();
				FVector RangeofBurstPoint = GetActorLocation() - HitCharacterLocation;

				if (RangeofBurstPoint.Length() < 100.0f)
				{
					//Near Range Damage Event
					UE_LOG(LogTemp, Log, TEXT("Grenade Near Range Damage Success"));
				}
				else
				{
					//Far Range Damage Event
					UE_LOG(LogTemp, Log, TEXT("Grenade Far Range Damage Success"));
				}
			}
		}
	}
}
