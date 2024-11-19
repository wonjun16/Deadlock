// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyUI.h"
#include "Components/TextBlock.h"
#include "Components/ScrollBox.h"
#include "Components/EditableTextBox.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "../GameMode/DeadlockGameState.h"
#include "../GameMode/DeadlockPlayerController.h"
#include "../GameMode/DeadlockPlayerState.h"
#include "../GameSystem/DeadlockGameInstanceSubsystem.h"

void ULobbyUI::NativeConstruct()
{
	Super::NativeConstruct(); 

	LeftTime = Cast<UTextBlock>(GetWidgetFromName(TEXT("LeftTime22")));
	AliveCount = Cast<UTextBlock>(GetWidgetFromName(TEXT("AliveCount22")));
	ChatScroll = Cast<UScrollBox>(GetWidgetFromName(TEXT("ChatScroll22")));
	ChatBox = Cast<UEditableTextBox>(GetWidgetFromName(TEXT("ChatBox22")));
	ReadyButton = Cast<UButton>(GetWidgetFromName(TEXT("ReadyButton22")));
	StartButton = Cast<UButton>(GetWidgetFromName(TEXT("StartButton22")));
	StartButton->SetIsEnabled(false);

	if (ChatBox)
	{
		ChatBox->OnTextCommitted.AddDynamic(this, &ULobbyUI::OnCommittedText);
	}
	if (ReadyButton)
	{
		ReadyButton->OnClicked.AddDynamic(this, &ULobbyUI::ProcessClicked);
	}
	if (StartButton)
	{
		StartButton->OnClicked.AddDynamic(this, &ULobbyUI::ProcessStartButtonClicked);
	}

	ADeadlockGameState* GS = Cast<ADeadlockGameState>(UGameplayStatics::GetGameState(GetWorld()));
	if (IsValid(GS))
	{
		GS->OnChangePlayerCount.AddDynamic(this, &ULobbyUI::SetAliveCount);
	}
}

void ULobbyUI::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void ULobbyUI::ProcessStartButtonClicked()
{

	UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(GetWorld());
	UDeadlockGameInstanceSubsystem* MySubsystem = GameInstance->GetSubsystem<UDeadlockGameInstanceSubsystem>();
	MySubsystem->InGame = true;
	ServerSetInGame();
	GetWorld()->ServerTravel(TEXT("DeadlockMap"));
}

void ULobbyUI::ServerSetInGame_Implementation()
{

	TArray<AActor*> PlayerControllers;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ADeadlockPlayerController::StaticClass(), PlayerControllers);

	for (AActor* Actor : PlayerControllers)
	{
		ADeadlockPlayerController* PC = Cast<ADeadlockPlayerController>(Actor);
		if (PC)
		{
			// 각 플레이어 컨트롤러에 대해 작업 수행
			PC->ClientSetInGame(); // 예: 클라이언트에게 알리기
		}
	}
}


void ULobbyUI::ProcessClicked()
{
	FButtonStyle ButtonStyle = ReadyButton->WidgetStyle;

	// 현재 색상 상태에 따라 색상 변경
	if (bIsReady)
	{
		// 회색으로 변경
		ButtonStyle.Normal.TintColor = FSlateColor(FLinearColor::Gray);
	}
	else
	{
		// 빨간색으로 변경
		ButtonStyle.Normal.TintColor = FSlateColor(FLinearColor::Red);
	}

	ReadyButton->SetStyle(ButtonStyle);
	bIsReady = !bIsReady;

	ADeadlockPlayerState* PS = GetOwningPlayerState<ADeadlockPlayerState>();
	ADeadlockPlayerController* PC = GetOwningPlayer<ADeadlockPlayerController>();
	if (IsValid(PS) && IsValid(PC))
	{
		PC->C2S_SendReadyState(!PS->bReadyState);
		UE_LOG(LogTemp, Warning, TEXT("State %d"), PS->bReadyState);
	}
}



void ULobbyUI::OnCommittedText(const FText& Text, ETextCommit::Type CommitMethod)
{
	switch (CommitMethod)
	{
	case ETextCommit::OnEnter:
	{
		ADeadlockPlayerController* PC = Cast<ADeadlockPlayerController>(GetOwningPlayer());
		if (IsValid(PC))
		{
			UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(GetWorld());
			UDeadlockGameInstanceSubsystem* MySubsystem = GameInstance->GetSubsystem<UDeadlockGameInstanceSubsystem>();
			FString Message = "";
			if (IsValid(MySubsystem))
			{
				Message = FString::Printf(TEXT("%s : %s"), *MySubsystem->UserName, *Text.ToString());
			}

			PC->C2S_SendMessage(FText::FromString(Message));
		}
		ChatBox->SetText(FText::FromString(TEXT("")));
		break;
	}
	case ETextCommit::OnCleared:
		ChatBox->SetUserFocus(GetOwningPlayer());
		break;
	}
}

void ULobbyUI::SetLeftTime(FString NewLeftTime)
{
	FString Temp = FString::Printf(TEXT("%s초 남았습니다."), *NewLeftTime);
	LeftTime->SetText(FText::FromString(Temp));
}

void ULobbyUI::SetAliveCount(uint32 NewAliveCount)
{
	FString Temp = FString::Printf(TEXT("%d명"), NewAliveCount);

	AliveCount->SetText(FText::FromString(Temp));
}

void ULobbyUI::AddChatMessage(FText AddMessage)
{
	UTextBlock* NewText = NewObject<UTextBlock>(ChatScroll);
	if (IsValid(NewText))
	{
		NewText->SetText(AddMessage);
		FSlateFontInfo NewFont = NewText->GetFont();
		NewFont.Size = 18.0f;
		NewText->SetFont(NewFont);
		ChatScroll->AddChild(NewText);
		ChatScroll->ScrollToEnd();
	}
}

void ULobbyUI::ShowStartrButton(bool Server)
{
	if (Server)
	{
		ReadyButton->SetVisibility(ESlateVisibility::Collapsed);
		StartButton->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		ReadyButton->SetVisibility(ESlateVisibility::Visible);
		StartButton->SetVisibility(ESlateVisibility::Collapsed);
	}
}
