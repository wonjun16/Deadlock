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

		//// 사운드 재생
		//if (FireSound)
		//{
		//	UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());
		//}
	}
}


void AWeaponBase::Grab(ACharacter* pOwnChar)
{
	//if (HasAuthority())
	{
		// 서버에서 직접 호출
		MulticastGrab(pOwnChar);
	}
	//else
	//{
	//	// 클라이언트에서 서버 호출
	//	ServerGrab(pOwnChar);
	//}
}

void AWeaponBase::ServerGrab_Implementation(ACharacter* pOwnChar)
{
	if (pOwnChar)
	{
		MulticastGrab(pOwnChar);
	}
}

void AWeaponBase::MulticastGrab_Implementation(ACharacter* pOwnChar)
{
	if (pOwnChar)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Green, "Grabbed the weapon!");
		Character = pOwnChar;
		Character->bUseControllerRotationYaw = true;

		WeaponMesh->SetSimulatePhysics(false);
		AttachToComponent(pOwnChar->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName("weapon"));
	}
}



