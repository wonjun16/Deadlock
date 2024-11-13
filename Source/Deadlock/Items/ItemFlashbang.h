// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemBase.h"
#include "ItemFlashbang.generated.h"

/**
 * 
 */
UCLASS()
class DEADLOCK_API AItemFlashbang : public AItemBase
{
	GENERATED_BODY()

public:
	AItemFlashbang();

	virtual void UseItem_Implementation(int ItemIndex)override;

	virtual void EventItemAffect_Implementation()override;

};
