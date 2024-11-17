// Fill out your copyright notice in the Description page of Project Settings.


#include "TitleGameMode.h"
#include "../Client/ClientSocket.h"
#include "Kismet/GameplayStatics.h"

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

void ATitleGameMode::OpenLobby()
{
	UGameplayStatics::OpenLevel(GEngine->GetWorld(), FName("Lobby"), true, ((FString)(L"Listen")));
}
