// Fill out your copyright notice in the Description page of Project Settings.

#include "MatchingRoomUI.h"
#include "Components/Button.h"
#include "Components/EditableTextBox.h"
#include "Kismet/GameplayStatics.h"

void UMatchingRoomUI::NativeConstruct()
{
	StartServerButton = Cast<UButton>(GetWidgetFromName(TEXT("WBPStartServerButton")));
	ConnectButton = Cast<UButton>(GetWidgetFromName(TEXT("WBPConnectButton")));
	ServerIP = Cast<UEditableTextBox>(GetWidgetFromName(TEXT("WBPServerIP")));
	UserName = Cast<UEditableTextBox>(GetWidgetFromName(TEXT("WBPUserName")));
	if (StartServerButton)
	{
		StartServerButton->OnClicked.AddDynamic(this, &UMatchingRoomUI::ProcessStartServerButtonClick);
	}

	if (ConnectButton)
	{
		ConnectButton->OnClicked.AddDynamic(this, &UMatchingRoomUI::ProcessConnectButtonClick);
	}

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("NativeConstruct"));
}

void UMatchingRoomUI::ProcessStartServerButtonClick()
{
	UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(GetWorld());
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("ProcessStartServerButtonClick"));
	//UMyGameInstanceSubsystem* MySubsystem = GameInstance->GetSubsystem<UMyGameInstanceSubsystem>();
	//if (IsValid(MySubsystem))
	//{
	//	MySubsystem->UserName = UserName->GetText().ToString();
	//}

	//UGameplayStatics::OpenLevel(GetWorld(), TEXT("Lobby"), true, TEXT("listen"));
}

void UMatchingRoomUI::ProcessConnectButtonClick()
{
	UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(GetWorld());
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("ProcessConnectButtonClick"));
	/*UMyGameInstanceSubsystem* MySubsystem = GameInstance->GetSubsystem<UMyGameInstanceSubsystem>();
	if (IsValid(MySubsystem))
	{
		MySubsystem->UserName = UserName->GetText().ToString();
	}*/
}
