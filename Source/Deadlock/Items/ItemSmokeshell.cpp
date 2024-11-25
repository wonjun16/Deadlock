// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemSmokeshell.h"

AItemSmokeshell::AItemSmokeshell()
{
	bReplicates = true;
	SetReplicateMovement(true);
	ItemTimer = 3.0f;
	EItemTypeIndex = 3;
}
