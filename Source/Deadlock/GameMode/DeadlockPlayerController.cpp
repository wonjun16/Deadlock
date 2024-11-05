// Fill out your copyright notice in the Description page of Project Settings.


#include "DeadlockPlayerController.h"
#include "../UI/LobbyUI.h"
#include "../GameMode/DeadlockPlayerState.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Components/Button.h"

void ADeadlockPlayerController::BeginPlay()
{
	Super::BeginPlay();

	FString Temp = FString::Printf(TEXT("%s %d %d"), *GetName(), GetLocalRole(), GetRemoteRole());
	UE_LOG(LogTemp, Warning, TEXT("%s"), *Temp);

	//#include 
	FSoftClassPath LobbyWidgetClassPath(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/LeeSangUk/Ui/WBP_CLoobyUI.WBP_CLoobyUI_C'"));

	//if (IsLocalPlayerController())
	//{
	//	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("IsLocalPlayerController"));
	//	UClass* WidgetClass = LobbyWidgetClassPath.TryLoadClass<ULobbyUI>();
	//	if (IsValid(WidgetClass))
	//	{
	//		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("IsValid(WidgetClass)"));
	//		LobbyWidget = CreateWidget<ULobbyUI>(this, WidgetClass);
	//		if (IsValid(LobbyWidget))
	//		{

	//			LobbyWidget->AddToViewport();
	//			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("AddToViewport"));
	//			SetInputMode(FInputModeGameAndUI());
	//			SetShowMouseCursor(true);

	//			/*if (GetLocalRole() == ENetRole::ROLE_Authority && GetRemoteRole() == ENetRole::ROLE_SimulatedProxy)
	//			{
	//				LobbyWidget->ShowStartrButton(true);
	//			}
	//			else
	//			{
	//				LobbyWidget->ShowStartrButton(false);
	//			}*/

	//		}
	//	}
	//}
}

bool ADeadlockPlayerController::C2S_SendMessage_Validate(const FText& Messsage)
{
	return false;
}

void ADeadlockPlayerController::C2S_SendMessage_Implementation(const FText& Message)
{
	for (auto Iter = GetWorld()->GetPlayerControllerIterator(); Iter; ++Iter)
	{
		ADeadlockPlayerController* PC = Cast< ADeadlockPlayerController>(*Iter);
		if (IsValid(PC))
		{
			PC->S2C_AddMessage(Message);
		}
	}
}

void ADeadlockPlayerController::S2C_AddMessage_Implementation(const FText& Messsage)
{
	//LobbyWidget->AddChatMessage(Messsage);
}

void ADeadlockPlayerController::C2S_SendReadyState_Implementation(bool NewState)
{
	//ADeadlockPlayerState* PS = GetPlayerState<ADeadlockPlayerState>();
	//if (IsValid(PS))
	//{
	//	//Server PlayerState 값변경, 자동 복제, Notify X
	//	PS->bReadyState = NewState;
	//	//Server
	//	PS->OnRep_ReadyState();

	//	uint8 TotalReadCount = 0;
	//	TArray<AActor*> FindActors;
	//	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ADeadlockPlayerState::StaticClass(), FindActors);
	//	for (auto FindActor : FindActors)
	//	{
	//		ADeadlockPlayerState* FindPS = Cast<ADeadlockPlayerState>(FindActor);
	//		if (IsValid(FindPS))
	//		{
	//			if (FindPS->bReadyState)
	//			{
	//				TotalReadCount++;
	//			}
	//		}
	//	}

	//	ADeadlockPlayerController* PC = Cast<ADeadlockPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	//	//Get Server PlayerController
	//	if (PC && PC->LobbyWidget)
	//	{
	//		//Start 활성화
	//		PC->LobbyWidget->StartButton->SetIsEnabled((FindActors.Num() - 1 == TotalReadCount)); // bIsEnabled = true;
	//	}
	//}
}
