// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HeathUI.generated.h"

/**
 * 
 */
UCLASS()
class DEADLOCK_API UHeathUI : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	float CurHP;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	float StockAmmo;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	int WeaponAmmo;
	
};
