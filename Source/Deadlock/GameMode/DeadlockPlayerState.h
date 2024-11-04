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

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Replicated)
	TArray<TObjectPtr<AActor>> EquipWeapon;

	/** Index of EquipWeapon and EquipWeaponType */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Replicated)
	uint8 CurEqiupWeapon;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Replicated)
	float HP;

	/** EWeaponType */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Replicated)
	TArray<uint8> EquipWeaponType;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Replicated)
	uint8 CurAmmo;

	UFUNCTION()
	bool IsCanReload();

	//로비에 쓰일 변수들
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State", ReplicatedUsing = "OnRep_ReadyState")
	uint8 bReadyState : 1;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION()
	void OnRep_ReadyState();
};
