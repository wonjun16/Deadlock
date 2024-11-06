// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "DeadlockGameInstanceSubsystem.generated.h"

/**
 * 
 */
UCLASS()
class DEADLOCK_API UDeadlockGameInstanceSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString UserName;

	bool InGame ;
	
};
