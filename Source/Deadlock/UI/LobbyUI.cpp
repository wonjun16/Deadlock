// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyUI.h"
#include "Components/TextBlock.h"
#include "Components/ScrollBox.h"
#include "Components/EditableTextBox.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"

void ULobbyUI::NativeConstruct()
{
	Super::NativeConstruct(); 

	LeftTime = Cast<UTextBlock>(GetWidgetFromName(TEXT("LeftTime")));
	AliveCount = Cast<UTextBlock>(GetWidgetFromName(TEXT("AliveCount")));
	ChatScroll = Cast<UScrollBox>(GetWidgetFromName(TEXT("ChatScroll")));
	ChatBox = Cast<UEditableTextBox>(GetWidgetFromName(TEXT("ChatBox")));
	ReadyButton = Cast<UButton>(GetWidgetFromName(TEXT("ReadyButton")));
	StartButton = Cast<UButton>(GetWidgetFromName(TEXT("StartButton")));

	//if (ChatBox)
	//{
	//	ChatBox->OnTextCommitted.AddDynamic(this, &ULobbyUI::OnCommittedText);
	//}
	//if (ReadyButton)
	//{
	//	ReadyButton->OnClicked.AddDynamic(this, &ULobbyUI::ProcessClicked);
	//}
	//if (StartButton)
	//{
	//	StartButton->OnClicked.AddDynamic(this, &ULobbyUI::ProcessStartButtonClicked);
	//}

	//ATPSGameStateBase* GS = Cast<ATPSGameStateBase>(UGameplayStatics::GetGameState(GetWorld()));
	/*if (IsValid(GS))
	{
		GS->OnChangePlayerCount.AddDynamic(this, &ULobbyUI::SetAliveCount);
	}*/
}

void ULobbyUI::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
}

void ULobbyUI::ProcessStartButtonClicked()
{
}

void ULobbyUI::ProcessClicked()
{
}

void ULobbyUI::OnCommittedText(const FText& Text, ETextCommit::Type CommitMethod)
{
}

void ULobbyUI::SetLeftTime(FString NewLeftTime)
{
}

void ULobbyUI::SetAliveCount(uint32 NewAliveCount)
{
}

void ULobbyUI::AddChatMessage(FText AddMessage)
{
}

void ULobbyUI::ShowStartrButton(bool Server)
{
}
