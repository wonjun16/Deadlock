// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemPainkiller.h"

AItemPainkiller::AItemPainkiller()
{
	EItemTypeIndex = 6;
}

void AItemPainkiller::StartItemTimer_Implementation()
{
	GetWorldTimerManager().SetTimer(ItemTriggerTimerHandle, this,
		&AItemBase::EventItemAffect_Implementation, 0.2f, false, 5.0f);
}