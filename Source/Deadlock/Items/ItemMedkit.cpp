// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemMedkit.h"
#include "Kismet/GameplayStatics.h"

AItemMedkit::AItemMedkit()
{
	bReplicates = true;
	SetReplicateMovement(true);
	EItemTypeIndex = 5;
}

void AItemMedkit::EventItemAffect_Implementation()
{
	DamageAmount = -100;
	UGameplayStatics::ApplyDamage(Owner, DamageAmount, Owner->GetInstigatorController(), this, 0);
	EndItemEvent();
}

void AItemMedkit::StartItemTimer_Implementation()
{
	GetWorldTimerManager().SetTimer(ItemTriggerTimerHandle, this,
		&AItemBase::EventItemAffect, 0.1f, false, 10.0f);
}