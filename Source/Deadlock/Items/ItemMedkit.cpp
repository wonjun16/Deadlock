// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemMedkit.h"
#include "Kismet/GameplayStatics.h"

AItemMedkit::AItemMedkit()
{
	bReplicates = true;
	SetReplicateMovement(true);
	ItemTimer = 8.0f;
	EItemTypeIndex = 5;
}

void AItemMedkit::ActivateAffect()
{
	DamageAmount = -70;
	UGameplayStatics::ApplyDamage(Owner, DamageAmount, Owner->GetInstigatorController(), this, 0);
}