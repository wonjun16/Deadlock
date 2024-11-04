// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemGrenade.h"
#include "Deadlock/Player/DeadlockCharacter.h"
#include "Deadlock/GameMode/DeadlockPlayerState.h"
#include "Deadlock/GameMode/DeadlockPlayerController.h"
#include "Kismet/GameplayStatics.h"

void AItemGrenade::EventItemAffect_Implementation()
{
	//Grenade Affect (Damage, Effect)

	TArray<FHitResult> HitActors;
	FVector GrenadeLocation = GetActorLocation();
	FVector Start = GrenadeLocation;
	FVector End = GrenadeLocation;

	FCollisionShape BlowRangeSphere = FCollisionShape::MakeSphere(500.0f);

	bool isHit = GetWorld()->SweepMultiByChannel(HitActors, Start, End,
		FQuat::Identity, ECC_WorldStatic, BlowRangeSphere);


	if (isHit)
	{
		for (auto& Hit : HitActors)
		{
			//UCapsuleComponent* CharacterCapsule = Cast<UCapsuleComponent>((Hit.GetActor())->GetRootComponent());
			//USkeletalMeshComponent* CharacterMesh = Cast<USkeletalMeshComponent>((Hit.GetActor()->GetRootComponent()));
			//UStaticMeshComponent* MeshComp = Cast<UStaticMeshComponent>((Hit.GetActor())->GetRootComponent());

			//Damage Event
			UE_LOG(LogTemp, Log, TEXT("GrenadeEvent Success"));
		}
	}
}
