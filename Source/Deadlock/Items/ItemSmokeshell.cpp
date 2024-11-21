// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemSmokeshell.h"

AItemSmokeshell::AItemSmokeshell()
{
	bReplicates = true;
	EItemTypeIndex = 3;
}

void AItemSmokeshell::StartItemTimer_Implementation()
{
	GetWorldTimerManager().SetTimer(ItemTriggerTimerHandle, this,
		&AItemBase::EventItemAffect, 0.3f, false, 3.3f);
}