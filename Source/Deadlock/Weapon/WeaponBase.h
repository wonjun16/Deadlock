// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameFramework/PlayerController.h"
#include "../Interface/WeaponInterface.h"
#include "WeaponBase.generated.h"

class ACharacter;

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
	uint8 MaxAmmo;

	UPROPERTY(VisibleAnywhere)
	uint8 CurAmmo;

	UPROPERTY(VisibleAnywhere)
	ACharacter* MyCharacter;

	UPROPERTY(VisibleAnywhere)
	EWeaponType WeaponType;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Weapon")
	TSubclassOf<AActor> ProjectileClass;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	void UseAmmo();
	void BindAmmo();
	void UnBindAmmo();
	void ChangeMag();
	void IsCanChangeMag(bool IsCanChangeMag);
	void GetShootDelayByRPM(float& DeltaTime);
	void SpawnProjectile();

	virtual void EventReloadTrigger_Implementation(bool bPress) override;

	virtual void EventReload_Implementation() override;

	virtual void EventAttackTrigger_Implementation(bool bPress) override;

	virtual void EventAttack_Implementation() override;

	virtual void EventSwitchWeaponTrigger_Implementation(bool bPress) override;

	virtual void EventSwitchWeapon_Implementation() override;

	virtual bool IsCanAttack_Implementation() override;

	virtual bool IsCanReload_Implementation() override;

	virtual bool IsCanSwitchWeapon_Implementation() override;

	virtual void EventDrop_Implementation(ACharacter* Character) override;

	virtual EWeaponType EventGrabWeapon_Implementation(ACharacter* Character) override;
};
