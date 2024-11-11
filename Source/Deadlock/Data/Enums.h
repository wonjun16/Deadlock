// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Enums.generated.h"

UCLASS()
class DEADLOCK_API AEnums : public AActor
{
	GENERATED_BODY()
};

// 1. Item Type Enums

UENUM(BlueprintType)
enum class EItemType : uint8
{
	E_Weapon = 0 		UMETA(DisplayName = "Weapon"),
	E_Ammo 				UMETA(DisplayName = "Ammo"),
	E_Grenade			UMETA(DisplayName = "Grenade"),
	E_Smokeshell		UMETA(DisplayName = "Smokeshell"),
	E_Flashbang 		UMETA(DisplayName = "Flashbang"),
	E_Medkit 			UMETA(DisplayName = "Medkit"),
	E_Painkiller 		UMETA(DisplayName = "Painkiller")
};

// 2. Weapon Type Enums

UENUM(BlueprintType)
enum class EWeaponType : uint8
{
	E_Hand = 0 			UMETA(DisplayName = "Hand"),
	E_Pistol 			UMETA(DisplayName = "Pistol"),
	E_SMG				UMETA(DisplayName = "SMG"),
	E_Rifle				UMETA(DisplayName = "Rifle"),
	E_Sniper 			UMETA(DisplayName = "Sniper"),
	E_ShotGun 			UMETA(DisplayName = "ShotGun"),
};

