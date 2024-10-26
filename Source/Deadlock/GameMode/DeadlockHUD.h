// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MagneticUI.h"
#include "HeathUI.h"
#include "GameFramework/HUD.h"
#include "DeadlockHUD.generated.h"

/**
 * 
 */
UCLASS()
class DEADLOCK_API ADeadlockHUD : public AHUD
{
	GENERATED_BODY()

public:
	// 위젯 인스턴스를 저장할 변수
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UMagneticUI* MagneticUIInstance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UHeathUI* HeathUIInstance;

	

};
