// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponBase.h"
#include "Kismet/GameplayStatics.h"
#include "Camera/PlayerCameraManager.h"
#include "Camera/CameraActor.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Engine/World.h"


// Sets default values
AWeaponBase::AWeaponBase()
{
	WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>("Weapon");
	WeaponMesh->SetCollisionProfileName("Weapon");
	WeaponMesh->SetSimulatePhysics(true);
	SetRootComponent(WeaponMesh);
	bReplicates = true; // Actor가 복제되도록 설정
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

void AWeaponBase::Fire()
{
	if (HasAuthority()) // 서버일 때
	{
		ExecuteFire();
	}
	else // 클라이언트일 때 서버에 요청
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Green, "124124123");
		ServerFire();
	}

}

// 서버에서 발사 요청
void AWeaponBase::ServerFire_Implementation()
{
	GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Green, "Fire");
	ExecuteFire(); // 서버에서 실제 발사
}

// 실제 발사 로직
void AWeaponBase::ExecuteFire()
{
	if (ProjectileClass)
	{
		// 총알 발사 로직
		FVector SpawnLocation = GetActorLocation(); // 발사 위치
		FRotator SpawnRotation = GetActorRotation(); // 발사 방향
		GetWorld()->SpawnActor<AActor>(ProjectileClass, SpawnLocation, SpawnRotation);
	}
}


bool AWeaponBase::EventReloadTrigger_Implementation(bool bPress)
{
	// 리로드 트리거 로직 작성
	if (bPress)
	{
		UE_LOG(LogTemp, Log, TEXT("Reload Trigger Pressed"));
		// 리로드 로직 수행
		return true;  // 리로드 성공 시 true 반환
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("Reload Trigger Released"));
		// 리로드 중단 등의 로직 수행
		return false;
	}
}

bool AWeaponBase::EventReload_Implementation()
{
	return false;
}

bool AWeaponBase::EventAttackTrigger_Implementation(bool bPress)
{
	return false;
}

bool AWeaponBase::EventAttack_Implementation()
{
	return false;
}

bool AWeaponBase::EventSwitchWeaponTrigger_Implementation(bool bPress)
{
	return false;
}

bool AWeaponBase::EventSwitchWeapon_Implementation()
{
	return false;
}

bool AWeaponBase::IsCanAttack_Implementation()
{
	return false;
}

bool AWeaponBase::IsCanReload_Implementation()
{
	return false;
}

bool AWeaponBase::IsCanSwitchWeapon_Implementation()
{
	return false;
}

bool AWeaponBase::EventDrop_Implementation(ACharacter* Character)
{
	return false;
}

EWeaponType AWeaponBase::EventGrabWeapon_Implementation(ACharacter* Character)
{

	GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Green, "ServerGrab_Implementation");

	MyCharacter = Character;
	MyCharacter->bUseControllerRotationYaw = true;

	WeaponMesh->SetSimulatePhysics(false);
	AttachToComponent(Character->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName("weapon"));


	return EWeaponType();
}

