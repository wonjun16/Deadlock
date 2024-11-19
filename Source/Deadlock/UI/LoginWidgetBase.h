// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Http.h"
#include "LoginWidgetBase.generated.h"

class UEditableTextBox;
class UButton;
class URegisterWidgetBase;
class UAlertWidgetBase;

/**
 * 
 */
UCLASS()
class DEADLOCK_API ULoginWidgetBase : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UEditableTextBox> IdEditBox;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UEditableTextBox> PasswordEditBox;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UButton> LoginButton;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UButton> MembershipButton;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<URegisterWidgetBase> RegisterWidget;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UAlertWidgetBase> FailLoginWidget;

	UFUNCTION()
	void LoginButtonClicked();

	UFUNCTION()
	void MembershipButtonClicked();

	bool IsValidID(const FString& ID);

	bool IsValidPassword(const FString& Password);

	void OnResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
};
