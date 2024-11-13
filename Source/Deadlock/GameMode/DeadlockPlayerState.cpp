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

    ItemCountsArray.Init(0, 7); // = { 0, 0, 0, 0, 0, 0, 0 }

    CurSelectItem = 2;
}

void ADeadlockPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ADeadlockPlayerState, HP);
    DOREPLIFETIME(ADeadlockPlayerState, EquipWeapon);
    DOREPLIFETIME(ADeadlockPlayerState, CurEqiupWeapon);
    DOREPLIFETIME(ADeadlockPlayerState, EquipWeaponType);
    DOREPLIFETIME(ADeadlockPlayerState, CurAmmo);
    DOREPLIFETIME(ADeadlockPlayerState, ItemCountsArray);
    DOREPLIFETIME(ADeadlockPlayerState, CurSelectItem);
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

void ADeadlockPlayerState::SelectItem(bool IsDirectionRight)
{
    if (IsDirectionRight)
    {
        if (CurSelectItem >= 6)
        {
            CurSelectItem = 2;
        }
        else
        {
            CurSelectItem++;
        }
        UE_LOG(LogTemp, Log, TEXT("Value : %d"), CurSelectItem);
    }
    else if (!IsDirectionRight)
    {
        if (CurSelectItem <= 2)
        {
            CurSelectItem = 6;
        }
        else
        {
            CurSelectItem--;
        }
        UE_LOG(LogTemp, Log, TEXT("Value : %d"), CurSelectItem);
    }
}

uint8 ADeadlockPlayerState::CalculateItemCount(bool IsAdd, uint8 ItemIndex)
{
    if (IsAdd)
    {
        //ItemEnumIndex Value = 2 ~ 6
        ItemCountsArray[ItemIndex]++;
        return ItemCountsArray[ItemIndex];
    }
    else if (!IsAdd && ItemCountsArray[CurSelectItem] > 0)
    {
        ItemCountsArray[CurSelectItem]--;
        return ItemCountsArray[CurSelectItem];
    }
    return 0;
}
