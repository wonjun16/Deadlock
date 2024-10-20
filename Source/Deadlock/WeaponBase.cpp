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
	bReplicates = true; // Actor�� �����ǵ��� ����
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
		FVector SpawnLocation = GetActorLocation() + GetActorForwardVector() * 100.0f; // �߻� ��ġ
		FRotator SpawnRotation = GetActorRotation(); // �߻� ����

		AActor* Projectile = GetWorld()->SpawnActor<AActor>(ProjectileClass, SpawnLocation, SpawnRotation);

		if (Projectile)
		{
			// �Ѿ��� �ӵ� ����
			UProjectileMovementComponent* ProjectileMovement = Projectile->FindComponentByClass<UProjectileMovementComponent>();
			if (ProjectileMovement)
			{
				ProjectileMovement->Velocity = GetActorForwardVector() * 1000.0f; // �߻� �ӵ�
			}
		}
	}
}

void AWeaponBase::Fire()
{
	if (HasAuthority()) // ������ ��
	{
		ExecuteFire();
	}
	else // Ŭ���̾�Ʈ�� �� ������ ��û
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Green, "124124123");
		ServerFire();
	}

}

// �������� �߻� ��û
void AWeaponBase::ServerFire_Implementation()
{
	GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Green, "Fire");
	ExecuteFire(); // �������� ���� �߻�
}

// ���� �߻� ����
void AWeaponBase::ExecuteFire()
{
	if (ProjectileClass)
	{
		// �Ѿ� �߻� ����
		FVector SpawnLocation = GetActorLocation(); // �߻� ��ġ
		FRotator SpawnRotation = GetActorRotation(); // �߻� ����
		GetWorld()->SpawnActor<AActor>(ProjectileClass, SpawnLocation, SpawnRotation);

		//// ���� ���
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
		// �������� ���� ȣ��
		MulticastGrab(pOwnChar);
	}
	//else
	//{
	//	// Ŭ���̾�Ʈ���� ���� ȣ��
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



