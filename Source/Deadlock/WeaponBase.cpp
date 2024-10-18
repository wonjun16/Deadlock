// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponBase.h"

// Sets default values
AWeaponBase::AWeaponBase()
{
	WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>("Weapon");
	WeaponMesh->SetCollisionProfileName("Weapon");
	WeaponMesh->SetSimulatePhysics(true);
	SetRootComponent(WeaponMesh);
}

// Called when the game starts or when spawned
void AWeaponBase::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AWeaponBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AWeaponBase::CalcStartForwadVector(int& a, int& b, int c)
{
}

void AWeaponBase::UseAmmo()
{
}

void AWeaponBase::BindAmmo()
{
}

void AWeaponBase::UnBindAmmo()
{
}

void AWeaponBase::ChangeMag()
{
}

void AWeaponBase::IsCanChangeMag(bool IsCanChangeMag)
{
}

void AWeaponBase::GetShootDelayByRPM(float& DeltaTime)
{
}

void AWeaponBase::Grab(ACharacter* pOwnChar)
{
	Character = pOwnChar;
	Character->bUseControllerRotationYaw = true;

	WeaponMesh->SetSimulatePhysics(false);
	AttachToComponent(pOwnChar->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName("weapon"));

	//OnUpdateAmmoToHud(m_Ammo);

}




