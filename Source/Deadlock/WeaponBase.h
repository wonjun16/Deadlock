// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/Character.h"
#include "Interface/WeaponInterface.h"
#include "Data/Enums.h"
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
	ACharacter* Character;

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

	UFUNCTION(BlueprintCallable, Category = "MyCategory")
	void Grab(ACharacter* pOwnChar);

	// �������� �߻��ϴ� �޼���
	UFUNCTION(Server, Reliable)
	void ServerGrab(ACharacter* pOwnChar);


	// Ŭ���̾�Ʈ���� ���� ��� �Լ�
	UFUNCTION(NetMulticast, Reliable)
	void MulticastGrab(ACharacter* pOwnChar);




};
