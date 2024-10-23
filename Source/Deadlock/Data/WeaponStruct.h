// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enums.h"
#include "WeaponStruct.generated.h"


class AWeaponBase;
class UStaticMesh;
class UAnimMontage;

/**
 * 
 */
class DEADLOCK_API WeaponStruct
{
public:
	WeaponStruct();
	~WeaponStruct();
};

USTRUCT(Atomic, BlueprintType)
struct FWeaponStruct : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AWeaponBase> WeaponClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Damage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint8 MaxAmmo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int RPM;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EWeaponType WeaponType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ReloadTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UStaticMesh> WeaponMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UAnimMontage> AttackMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UAnimMontage> ReloadMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UAnimMontage> SwitchMontage;
};