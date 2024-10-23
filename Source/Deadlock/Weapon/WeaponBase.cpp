// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponBase.h"
#include "Kismet/GameplayStatics.h"
#include "Camera/PlayerCameraManager.h"
#include "Camera/CameraActor.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Engine/World.h"
#include "GameFramework/Character.h"
#include "../Data/Enums.h"
#include "Engine/DataTable.h"


// Sets default values
AWeaponBase::AWeaponBase()
{
	static ConstructorHelpers::FObjectFinder<UDataTable> DataTable(TEXT("/Script/Engine.DataTable'/Game/Deadlock/Data/DT_WeaponData.DT_WeaponData'"));
	if (DataTable.Succeeded())
	{
		WeaponData = DataTable.Object;
		static const FString ContextString(TEXT("GENERAL"));
		Row = WeaponData->FindRow<FWeaponStruct>(FName(TEXT("Rifle")), ContextString);
	}
	WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>("Weapon");
	SetRootComponent(WeaponMesh);
	WeaponMesh->SetSimulatePhysics(true);
	WeaponMesh->SetCollisionProfileName(TEXT("Weapon"));
	bReplicates = true;

	MaxAmmo = 30;
	CurAmmo = 0;
	MyCharacter = nullptr;
	WeaponType = EWeaponType::E_Rifle;
	ProjectileClass = nullptr;
	
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

void AWeaponBase::SpawnProjectile()
{
	if (ProjectileClass)
	{
		FVector SpawnLocation = GetActorLocation() + GetActorForwardVector() * 100.0f; // 발사 위치
		FRotator SpawnRotation = GetActorRotation(); // 발사 각도

		AActor* Projectile = GetWorld()->SpawnActor<AActor>(ProjectileClass, SpawnLocation, SpawnRotation);

		if (Projectile)
		{
			// 총알의 속도 설정
			UProjectileMovementComponent* ProjectileMovement = Projectile->FindComponentByClass<UProjectileMovementComponent>();
			if (ProjectileMovement)
			{
				ProjectileMovement->Velocity = GetActorForwardVector() * 1000.0f; // 발사 속도
			}
		}
	}
}

void AWeaponBase::EventReloadTrigger_Implementation(bool bPress)
{

}

void AWeaponBase::EventReload_Implementation()
{

}

void AWeaponBase::EventAttackTrigger_Implementation(bool bPress)
{

}

void AWeaponBase::EventAttack_Implementation()
{

}

void AWeaponBase::EventSwitchWeaponTrigger_Implementation(bool bPress)
{

}

void AWeaponBase::EventSwitchWeapon_Implementation()
{

}

bool AWeaponBase::IsCanAttack_Implementation()
{
	return false;
}

bool AWeaponBase::IsCanReload_Implementation()
{
	bool bCanReload = false;
	
	if (GEngine && Row)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Green, FString::Printf(TEXT("%d"), Row->MaxAmmo));
	}

	//몽타주 플레이중 아닐때
	return false;
}

bool AWeaponBase::IsCanSwitchWeapon_Implementation()
{
	return false;
}

void AWeaponBase::EventDrop_Implementation(ACharacter* Character)
{
	WeaponMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	WeaponMesh->SetSimulatePhysics(true);
	WeaponMesh->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
	MyCharacter = nullptr;
}

EWeaponType AWeaponBase::EventGrabWeapon_Implementation(ACharacter* Character)
{

	GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Green, "Weapon Grab Implementation");

	MyCharacter = Character;
	//MyCharacter->bUseControllerRotationYaw = true;

	WeaponMesh->SetSimulatePhysics(false);
	WeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	AttachToComponent(Character->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName("weapon"));

	return WeaponType;
}

