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
	// 플레이어별 위치 배열 (동기화가 필요한 변수에 Replicated 추가)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn Locations", Replicated)
	TArray<FVector> SpawnLocations;

	// SpawnIndex를 Replicated로 설정
	UPROPERTY(Replicated)
	int32 SpawnIndex;

	// Replication 관련 함수
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

};
