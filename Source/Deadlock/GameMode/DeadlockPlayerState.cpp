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

    ItemCountsArray.Init(5, 7); //Set 5 just for test

    CurSelectItemIndex = 2;
    
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
    DOREPLIFETIME(ADeadlockPlayerState, ItemCountsArray);
    DOREPLIFETIME(ADeadlockPlayerState, CurSelectItemIndex);
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

void ADeadlockPlayerState::SelectItem_Implementation(bool IsDirectionRight)
{
    if (IsDirectionRight)
    {
        if (CurSelectItemIndex >= 6)
        {
            CurSelectItemIndex = 2;
        }
        else
        {
            CurSelectItemIndex++;
        }
        UE_LOG(LogTemp, Log, TEXT("Value : %d"), CurSelectItemIndex);
    }
    else if (!IsDirectionRight)
    {
        if (CurSelectItemIndex <= 2)
        {
            CurSelectItemIndex = 6;
        }
        else
        {
            CurSelectItemIndex--;
        }
        UE_LOG(LogTemp, Log, TEXT("Value : %d"), CurSelectItemIndex);
    }
}

uint8 ADeadlockPlayerState::CalculateItemCount(bool IsAdd, uint8 ItemIndex)
{
    if (IsAdd)
    { 
        if (ItemCountsArray[ItemIndex] < 10)
        {
            ItemCountsArray[ItemIndex]++;
        }
        return ItemCountsArray[ItemIndex];
    }
    else if (!IsAdd && ItemCountsArray[CurSelectItemIndex] > 0)
    {
        ItemCountsArray[CurSelectItemIndex]--;
        return ItemCountsArray[CurSelectItemIndex];
    }
    return 0;
}

void ADeadlockPlayerState::OnRep_ReadyState()
{
    UE_LOG(LogTemp, Warning, TEXT("%s"), *GetName());
}
