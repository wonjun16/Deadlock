// Fill out your copyright notice in the Description page of Project Settings.


#include "DeadlockPlayerState.h"

ADeadlockPlayerState::ADeadlockPlayerState()
{
    EquipWeapon = TArray<TObjectPtr<AActor>>();

    CurEqiupWeapon = 0;

    HP = 100.0f;

    EquipWeaponType = TArray<uint8>();
}
