// Fill out your copyright notice in the Description page of Project Settings.


#include "TitleGameMode.h"
#include "../Client/ClientSocket.h"

void ATitleGameMode::StartPlay()
{
	Super::StartPlay();

	UE_LOG(LogTemp, Log, TEXT("Start Play"));

	ClientSocket::ConnectSocket();
}

void ATitleGameMode::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	UE_LOG(LogTemp, Log, TEXT("End Play"));

	//ClientSocket::DisconnectSocket();

	Super::EndPlay(EndPlayReason);
}
