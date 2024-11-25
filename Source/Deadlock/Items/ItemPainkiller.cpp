// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemPainkiller.h"
#include "Kismet/GameplayStatics.h"

AItemPainkiller::AItemPainkiller()
{
	bReplicates = true;
	SetReplicateMovement(true);
	ItemTimer = 5.0f;
	EItemTypeIndex = 6;
}

void AItemPainkiller::EventItemAffect_Implementation()
{
	DamageAmount = -30;
	UGameplayStatics::ApplyDamage(Owner, DamageAmount, Owner->GetInstigatorController(), this, 0);
	EndItemEvent();
}
