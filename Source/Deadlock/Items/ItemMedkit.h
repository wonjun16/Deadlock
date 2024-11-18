// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemBase.h"
#include "ItemMedkit.generated.h"

/**
 * 
 */
UCLASS()
class DEADLOCK_API AItemMedkit : public AItemBase
{
	GENERATED_BODY()

public:
	AItemMedkit();

	virtual void EventItemAffect_Implementation()override;

	virtual void StartItemTimer_Implementation()override;
	
};
