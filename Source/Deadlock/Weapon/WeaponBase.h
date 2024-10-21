// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/Character.h"
#include "../Interface/WeaponInterface.h"
#include "../Data/Enums.h"
#include "WeaponBase.generated.h"

UCLASS()
class DEADLOCK_API AWeaponBase : public AActor, public IWeaponInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeaponBase();

UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* WeaponMesh;

UPROPERTY(VisibleAnywhere)
	int32 Ammo;

UPROPERTY(VisibleAnywhere)
	ACharacter* MyCharacter;

UPROPERTY(VisibleAnywhere)
	EWeaponType WeaponData;

UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Weapon")
	TSubclassOf<AActor> ProjectileClass; // �߻��� �Ѿ� Ŭ����

	// �� �Ҹ� ����
UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	USoundBase* FireSound; // �� �Ҹ� ����

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	void CalcStartForwadVector(FVector& StartVec, FVector& EndVec, FVector MuzzleLoc);
	void UseAmmo();
	void BindAmmo();
	void UnBindAmmo();
	void ChangeMag();
	void IsCanChangeMag(bool IsCanChangeMag);
	void GetShootDelayByRPM(float& DeltaTime);
	void SpawnProjectile();

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void Fire();
	// �������� �߻��ϴ� �޼���
	UFUNCTION(Server, Reliable)
	void ServerFire();
	// ���� �߻� ����
	void ExecuteFire();


	virtual bool EventReloadTrigger_Implementation(bool bPress) override;

	// EventReload function implementation
	virtual bool EventReload_Implementation() override;

	// EventAttackTrigger function implementation
	virtual bool EventAttackTrigger_Implementation(bool bPress) override;

	// EventAttack function implementation
	virtual bool EventAttack_Implementation() override;

	// EventSwitchWeaponTrigger function implementation
	virtual bool EventSwitchWeaponTrigger_Implementation(bool bPress) override;

	// EventSwitchWeapon function implementation
	virtual bool EventSwitchWeapon_Implementation() override;

	// IsCanAttack function implementation
	virtual bool IsCanAttack_Implementation() override;

	// IsCanReload function implementation
	virtual bool IsCanReload_Implementation() override;

	// IsCanSwitchWeapon function implementation
	virtual bool IsCanSwitchWeapon_Implementation() override;

	// EventDrop function implementation
	virtual bool EventDrop_Implementation(ACharacter* Character) override;

	// EventGrabWeapon function implementation
	virtual EWeaponType EventGrabWeapon_Implementation(ACharacter* Character) override;



};
