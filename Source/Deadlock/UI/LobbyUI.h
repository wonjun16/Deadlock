// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LobbyUI.generated.h"

class UScrollBox;
class UEditableTextBox;
class UTextBlock;
class UButton;

/**
 * 
 */
UCLASS()
class DEADLOCK_API ULobbyUI : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
	UTextBlock* LeftTime;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
	UTextBlock* AliveCount;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
	UScrollBox* ChatScroll;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
	UEditableTextBox* ChatBox;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
	UButton* ReadyButton;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
	UButton* StartButton;


	UFUNCTION()
	void ProcessStartButtonClicked();

	UFUNCTION()
	void ProcessClicked();

	//UFUNCTION()
	//void OnCommittedText(const FText& Text, ETextCommit::Type CommitMethod);

	void SetLeftTime(FString NewLeftTime);

	UFUNCTION()
	void SetAliveCount(uint32 NewAliveCount);

	void AddChatMessage(FText AddMessage);

	void ShowStartrButton(bool Server);
	
};
