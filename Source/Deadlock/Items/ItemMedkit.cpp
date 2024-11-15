// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemMedkit.h"

AItemMedkit::AItemMedkit()
{
	EItemTypeIndex = 5;
}

void AItemMedkit::StartItemTimer_Implementation()
{
	GetWorldTimerManager().SetTimer(ItemTriggerTimerHandle, this,
		&AItemBase::EventItemAffect_Implementation, 0.1f, false, 10.0f);
}