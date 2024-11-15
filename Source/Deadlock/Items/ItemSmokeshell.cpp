// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemSmokeshell.h"

AItemSmokeshell::AItemSmokeshell()
{
	EItemTypeIndex = 3;
}

void AItemSmokeshell::StartItemTimer_Implementation()
{
	GetWorldTimerManager().SetTimer(ItemTriggerTimerHandle, this,
		&AItemBase::EventItemAffect_Implementation, 0.3f, false, 3.3f);
}