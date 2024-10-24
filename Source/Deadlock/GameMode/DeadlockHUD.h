// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "UI/MagneticUI.h"
#include "DeadlockHUD.generated.h"

/**
 * 
 */
UCLASS()
class DEADLOCK_API ADeadlockHUD : public AHUD
{
	GENERATED_BODY()

	// 위젯 인스턴스를 저장할 변수
	UPROPERTY()
	UMagneticUI* MyWidgetInstance;

	

};
