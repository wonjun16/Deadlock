// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "WeaponInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UWeaponInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class DEADLOCK_API IWeaponInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void EventReloadTrigger(bool bPress);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void EventReload();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void EventAttackTrigger(bool bPress);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void EventAttack();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void EventSwitchWeaponTrigger(bool bPress);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void EventSwitchWeapon();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	bool IsCanAttack();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	bool IsCanReload();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	bool IsCanSwitchWeapon();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void EventDrop(ACharacter* Character);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	EWeaponType EventGrabWeapon(ACharacter* Character);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	FVector GetIronSightLoc();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void UseAmmo();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	int GetCurAmmo();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void SpawnBullet();
};
