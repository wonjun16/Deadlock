// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Spawner.generated.h"

UCLASS()
class DEADLOCK_API ASpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASpawner();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	// �÷��̾ ��ġ �迭 (����ȭ�� �ʿ��� ������ Replicated �߰�)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn Locations", Replicated)
	TArray<FVector> SpawnLocations;

	// SpawnIndex�� Replicated�� ����
	UPROPERTY(Replicated)
	int32 SpawnIndex;

	// Replication ���� �Լ�
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

};
