// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "http.h"
#include "RegisterWidgetBase.generated.h"

class UEditableTextBox;
class UButton;
class UAlertWidgetBase;

/**
 * 
 */
UCLASS()
class DEADLOCK_API URegisterWidgetBase : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UEditableTextBox> IdEditBox;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UEditableTextBox> PasswordEditBox;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UEditableTextBox> NickNameEditBox;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UButton> RegisterButton;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UButton> GoBackButton;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UAlertWidgetBase> SuccessRegisterWidget;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UAlertWidgetBase> FailRegisterWidget;

	UFUNCTION()
	void RegisterButtonClicked();

	UFUNCTION()
	void GoBackButtonClicked();

	//ID : 6~20자 / 특수문자 포함 X  / 공백 포함 X
	bool IsValidID(const FString& ID);

	//Password : 6~20자 / 무조건 특수문자 1개 이상 포함 / 공백 포함 X
	bool IsValidPassword(const FString& Password);

	//NickName : 3~20자 / 특수문자 포함 X  / 공백 포함 X
	bool IsValidNickName(const FString& NickName);

	void OnResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
};
