// Fill out your copyright notice in the Description page of Project Settings.


#include "DeadlockPlayerState.h"
#include "Net/UnrealNetwork.h"
#include "../Interface/WeaponInterface.h"

ADeadlockPlayerState::ADeadlockPlayerState()
{
    EquipWeapon.Init(nullptr, 2);

    CurEqiupWeapon = 0;

    HP = 100.0f;

    EquipWeaponType.Init(0, 2);

    CurAmmo = 100;

    bReadyState = false;

  
}

void ADeadlockPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ADeadlockPlayerState, HP);
    DOREPLIFETIME(ADeadlockPlayerState, EquipWeapon);
    DOREPLIFETIME(ADeadlockPlayerState, CurEqiupWeapon);
    DOREPLIFETIME(ADeadlockPlayerState, EquipWeaponType);
    DOREPLIFETIME(ADeadlockPlayerState, CurAmmo);
    DOREPLIFETIME(ADeadlockPlayerState, bReadyState);
}

bool ADeadlockPlayerState::IsCanReload()
{
    bool Reloadable = false;
    if (EquipWeapon[CurEqiupWeapon])
    {
        IWeaponInterface* ICurWeapon = Cast<IWeaponInterface>(EquipWeapon[CurEqiupWeapon]);
        Reloadable = ICurWeapon->Execute_IsCanReload(EquipWeapon[CurEqiupWeapon]);
    }
    else
    {
        Reloadable = false;
    }
    
    if (CurAmmo <= 0)
    {
        Reloadable = false;
    }

    return Reloadable;
}


void ADeadlockPlayerState::OnRep_ReadyState()
{
    UE_LOG(LogTemp, Warning, TEXT("%s"), *GetName());
}