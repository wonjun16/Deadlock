// Fill out your copyright notice in the Description page of Project Settings.


#include "TitleGameMode.h"

void ATitleGameMode::StartPlay()
{
	Super::StartPlay();

	UE_LOG(LogTemp, Log, TEXT("Start Play"));
}

void ATitleGameMode::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	UE_LOG(LogTemp, Log, TEXT("End Play"));

	Super::EndPlay(EndPlayReason);
}
