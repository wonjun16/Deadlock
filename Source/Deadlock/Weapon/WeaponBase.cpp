// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponBase.h"
#include "Kismet/GameplayStatics.h"
#include "Camera/PlayerCameraManager.h"
#include "Camera/CameraActor.h"
#include "Kismet/KismetMathLibrary.h"
#include "Engine/World.h"
#include "GameFramework/Character.h"
#include "../Data/Enums.h"
#include "Engine/DataTable.h"
#include "Bullet.h"
#include "GameFramework/ProjectileMovementComponent.h"

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
	SetReplicateMovement(true);

	MaxAmmo = 30;
	CurAmmo = 29;
	MyCharacter = nullptr;
	WeaponType = EWeaponType::E_Rifle;
	Bullet = nullptr;
	
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

FVector AWeaponBase::CalcStartForwadVector(FVector MuzzleLoc)
{
	APlayerCameraManager* CameraManager = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0);

	FVector ActorLocation = CameraManager->GetCameraLocation();
	FVector ActorForwardVector = CameraManager->GetActorForwardVector();

	float Distance = UKismetMathLibrary::Vector_Distance(MuzzleLoc, ActorLocation);


	FVector MultiplyVector = ActorForwardVector * Distance;
	FVector StartVector = ActorLocation + MultiplyVector;

	return StartVector;
}

void AWeaponBase::SpawnBullet(FVector SpawnLocation, FRotator SpawnRotation)
{
	Bullet = GetWorld()->SpawnActor<ABullet>(BulletClass, SpawnLocation, SpawnRotation);
	FVector Direction = SpawnRotation.Vector();
	if (Bullet && Direction.Normalize())
	{
		Bullet->Fire(Direction);
	}
}

void AWeaponBase::EventReloadTrigger_Implementation(bool bPress)
{
	if (MyCharacter && Row)
	{
		MyCharacter->PlayAnimMontage(Row->ReloadMontage);
	}
}

void AWeaponBase::EventReload_Implementation()
{

}

void AWeaponBase::EventAttackTrigger_Implementation(bool bPress)
{
	if (Execute_IsCanAttack(this) && bPress && MyCharacter && Row)
	{
		MyCharacter->PlayAnimMontage(Row->AttackMontage);
		//몽타주에서 실행으로 교체
		Execute_EventAttack(this);
	}
}

void AWeaponBase::EventAttack_Implementation()
{
	if (Execute_IsCanAttack(this) && WeaponMesh->DoesSocketExist(FName(TEXT("muzzle"))))
	{
		FTransform MuzzleTransform = WeaponMesh->UStaticMeshComponent::GetSocketTransform(FName(TEXT("muzzle")));
		FVector SpawnLocation = CalcStartForwadVector(MuzzleTransform.GetLocation());

		//emitter, sound
		if (BulletClass)
		{
			SpawnBullet(SpawnLocation, MuzzleTransform.Rotator());
		}
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Green, "Error : there is no muzzle");
	}
}

void AWeaponBase::EventSwitchWeaponTrigger_Implementation(bool bPress)
{

}

void AWeaponBase::EventSwitchWeapon_Implementation()
{

}

bool AWeaponBase::IsCanAttack_Implementation()
{
	bool bAttackable = false;

	if (CurAmmo > 0)
	{
		bAttackable = true;
	}

	return bAttackable;
}

bool AWeaponBase::IsCanReload_Implementation()
{
	bool bCanReload = true;
	
	if (Row)
	{
		if (Row->MaxAmmo <= CurAmmo)
		{
			bCanReload = false;
		}

		if (MyCharacter)
		{
			if (MyCharacter->GetMesh()->GetAnimInstance()->GetCurrentActiveMontage() == Row->ReloadMontage)
			{
				bCanReload = false;
			}
		}
	}
	return bCanReload;
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

	WeaponMesh->SetSimulatePhysics(false);
	WeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	AttachToComponent(Character->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName("weapon"));

	return WeaponType;
}

FVector AWeaponBase::GetIronSightLoc_Implementation()
{
	FVector IronSightLoc(0, 0, 0);
	if (WeaponMesh->DoesSocketExist(FName(TEXT("ironsight"))))
	{
		IronSightLoc = WeaponMesh->GetSocketLocation(FName(TEXT("ironsight")));
	}
	return IronSightLoc;
}

