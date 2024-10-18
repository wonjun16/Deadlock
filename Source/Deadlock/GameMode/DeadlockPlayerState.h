// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "DeadlockPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class DEADLOCK_API ADeadlockPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	ADeadlockPlayerState();

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<TObjectPtr<AActor>> EquipWeapon;

	/** Index of EquipWeapon and EquipWeaponType */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	uint8 CurEqiupWeapon;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float HP;

	/** EWeaponType */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<uint8> EquipWeaponType;
};
