// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "DeadlockGameState.generated.h"

/**
 * 
 */

DECLARE_DYNAMIC_MULTICAST_SPARSE_DELEGATE_OneParam(FChangePlayerCount, ADeadlockGameState, OnChangePlayerCount, uint32, NewAliveCount);
UCLASS()
class DEADLOCK_API ADeadlockGameState : public AGameStateBase
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data", ReplicatedUsing = "OnRep_PlayerCount")
	uint8 PlayerCount = 0;

	UFUNCTION()
	void OnRep_PlayerCount();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data", ReplicatedUsing = "OnRep_LeftTime")
	uint8 LeftTime = 60;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data", Replicated)
	float RemainTime= 5;

	UFUNCTION()
	void OnRep_LeftTime();

	void DecreaseLeftTime();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UPROPERTY(BlueprintAssignable, Category = "Data")
	FChangePlayerCount OnChangePlayerCount;
};

