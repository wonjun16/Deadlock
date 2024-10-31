// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MatchingRoomUI.generated.h"


class UButton;
class UEditableTextBox;


/** 
 * 
 */
UCLASS()
class DEADLOCK_API UMatchingRoomUI : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UButton> StartServerButton;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UButton> ConnectButton;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UEditableTextBox> ServerIP;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UEditableTextBox> UserName;

	UFUNCTION()
	void ProcessStartServerButtonClick();

	UFUNCTION()
	void ProcessConnectButtonClick();
	
};
