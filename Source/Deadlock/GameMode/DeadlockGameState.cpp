// Fill out your copyright notice in the Description page of Project Settings.


#include "DeadlockGameState.h"
#include "Net/UnrealNetwork.h"
#include "../GameMode/DeadlockPlayerController.h"
#include "../UI/LobbyUI.h"
#include "Kismet/GameplayStatics.h"

void ADeadlockGameState::OnRep_PlayerCount()
{
	if (OnChangePlayerCount.IsBound())
	{
		OnChangePlayerCount.Broadcast((uint32)PlayerCount);
	}
}

void ADeadlockGameState::OnRep_LeftTime()
{
	//ADeadlockPlayerController* PC = Cast<ADeadlockPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	//if (IsValid(PC) && IsValid(PC->LobbyWidget))
	//{
	//	FString Temp = FString::Printf(TEXT("%d"), LeftTime);
	//	PC->LobbyWidget->SetLeftTime(Temp);
	//}
}

void ADeadlockGameState::DecreaseLeftTime()
{
	LeftTime--;
	LeftTime = FMath::Clamp(LeftTime, 0, 60);
}

void ADeadlockGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ADeadlockGameState, PlayerCount);
	DOREPLIFETIME(ADeadlockGameState, LeftTime);
}
