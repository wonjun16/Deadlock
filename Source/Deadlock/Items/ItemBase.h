// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Deadlock/Interface/ItemInterface.h"
#include "ItemBase.generated.h"

class UStaticMeshComponent;
class UNiagaraSystem;
class UNiagaraComponent;

UCLASS()
class DEADLOCK_API AItemBase : public AActor, public IItemInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AItemBase();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* ItemMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UNiagaraComponent* ItemBaseEffect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UNiagaraSystem* EffectAsset;

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

	virtual void UseItem_Implementation(int CurrentItemCount)override;

	virtual void GetItem_Implementation(int CurrentItemCount, int MaxItemCount)override;

	virtual void PlayItemEffect_Implementation()override;

};
