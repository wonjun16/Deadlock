// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponBase.h"
#include "Kismet/GameplayStatics.h"
#include "Camera/PlayerCameraManager.h"
#include "Camera/CameraActor.h"
#include "Kismet/KismetMathLibrary.h"


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

void AWeaponBase::CalcStartForwadVector(FVector& StartVec, FVector& EndVec, FVector MuzzleLoc)
{
	APlayerCameraManager* CameraManager = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0);

	FVector ActorLocation = CameraManager->GetCameraLocation();
	FVector ActorForwardVector = CameraManager->GetActorForwardVector();

	float Distance = UKismetMathLibrary::Vector_Distance(MuzzleLoc, ActorLocation);


	FVector MultiplyVector = ActorForwardVector * Distance;
	FVector PlusVector = ActorLocation + MultiplyVector;
	FVector Multiply5000Vector = ActorForwardVector * 5000;
	FVector PlusVector2 = ActorLocation + Multiply5000Vector;

	StartVec = PlusVector;
	MuzzleLoc = PlusVector2;
}

void AWeaponBase::UseAmmo()
{
	Ammo = Ammo - 1;
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




