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
	UParticleSystem* ItemParticle;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	FTimerHandle ItemTriggerTimerHandle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemTime")
	float ItemTimer;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
	uint8 EItemTypeIndex;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
	uint8 CurrentItemCount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
	float DamageAmount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
	bool bIsCanBeDetroy;

	UFUNCTION(Server, Reliable)
	void EventItemAffect();
	void EventItemAffect_Implementation();

	UFUNCTION(NetMulticast, Reliable)
	void ClientItemAffect();
	void ClientItemAffect_Implementation();

	UFUNCTION(Server, Reliable)
	void ServerPlayEffect();
	void ServerPlayEffect_Implementation();

	UFUNCTION(NetMulticast, Reliable)
	void PlayItemEffect();
	void PlayItemEffect_Implementation();

	UFUNCTION(NetMulticast, Reliable)
	void EndItemEvent();
	void EndItemEvent_Implementation();

};
