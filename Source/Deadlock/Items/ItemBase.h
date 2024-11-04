// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Deadlock/Interface/ItemInterface.h"
#include "ItemBase.generated.h"

class UCapsuleComponent;
class UStaticMeshComponent;
class USceneComponent;
class UNiagaraSystem;
class UNiagaraComponent;
class UGameplayStatics;

UCLASS()
class DEADLOCK_API AItemBase : public AActor, public IItemInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AItemBase();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UCapsuleComponent* CapsuleCollision;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* ItemMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UNiagaraComponent* ItemBaseEffect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UNiagaraSystem* EffectAsset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USceneComponent* ImpulsePosition;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Component")
	uint8 MaxItemCount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Component")
	uint8 CurrentItemCount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Component")
	float HealAmount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Component")
	float DamageAmount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Component")
	float ThrowSpeed;


	virtual void UseItem_Implementation(int CurrentItemCount)override;

	virtual void GetItem_Implementation(int CurrentItemCount, int MaxItemCount)override;

	virtual void PlayItemEffect_Implementation()override;

	virtual void ThrowMovement_Implementation(FVector ThrowDirection)override;

	virtual void EventItemAffect_Implementation()override;
};
