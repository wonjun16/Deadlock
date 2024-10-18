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
	bool EventReloadTrigger(bool bPress);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	bool EventReload();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	bool EventAttackTrigger(bool bPress);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	bool EventAttack();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	bool EventSwitchWeaponTrigger(bool bPress);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	bool EventSwitchWeapon();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	bool IsCanAttack();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	bool IsCanReload();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	bool IsCanSwitchWeapon();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	bool EventDrop(ACharacter* Character);
};
