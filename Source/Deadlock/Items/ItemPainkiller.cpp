// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemPainkiller.h"
#include "Kismet/GameplayStatics.h"

AItemPainkiller::AItemPainkiller()
{
	bReplicates = true;
	SetReplicateMovement(true);
	EItemTypeIndex = 6;
}

void AItemPainkiller::EventItemAffect_Implementation()
{
	DamageAmount = -30;
	UGameplayStatics::ApplyDamage(Owner, DamageAmount, Owner->GetInstigatorController(), this, 0);
	EndItemEvent();
}

void AItemPainkiller::StartItemTimer_Implementation()
{
	GetWorldTimerManager().SetTimer(ItemTriggerTimerHandle, this,
		&AItemBase::EventItemAffect, 0.2f, false, 5.0f);
}