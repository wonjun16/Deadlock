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
	CurAmmo = 29;
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
	}
}

void AWeaponBase::EventAttack_Implementation()
{
	if (Execute_IsCanAttack(this))
	{
		WeaponMesh->GetStaticMesh();
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
	//MyCharacter->bUseControllerRotationYaw = true;

	WeaponMesh->SetSimulatePhysics(false);
	WeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	AttachToComponent(Character->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName("weapon"));

	return WeaponType;
}

