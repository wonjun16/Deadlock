// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemBase.h"
#include "ItemGrenade.generated.h"

/**
 * 
 */
UCLASS()
class DEADLOCK_API AItemGrenade : public AItemBase
{
	GENERATED_BODY()

public:
	AItemGrenade();

	virtual void ActivateAffect() override;

};
