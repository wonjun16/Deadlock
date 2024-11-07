// Fill out your copyright notice in the Description page of Project Settings.


#include "LoginWidgetBase.h"
#include "Components/EditableTextBox.h"
#include "Components/Button.h"
#include "regex"

void ULoginWidgetBase::NativeConstruct()
{
	Super::NativeConstruct();

	IdEditBox = Cast<UEditableTextBox>(GetWidgetFromName(TEXT("BP_IdEditBox")));
	PasswordEditBox = Cast<UEditableTextBox>(GetWidgetFromName(TEXT("BP_PasswordEditBox")));
	LoginButton = Cast<UButton>(GetWidgetFromName(TEXT("BP_LoginButton")));
	MembershipButton = Cast<UButton>(GetWidgetFromName(TEXT("BP_MembershipButton")));

	if (LoginButton)
	{
		LoginButton->OnClicked.AddDynamic(this, &ULoginWidgetBase::LoginButtonClicked);
	}
	if (MembershipButton)
	{
		MembershipButton->OnClicked.AddDynamic(this, &ULoginWidgetBase::MembershipButtonClicked);
	}
}

void ULoginWidgetBase::LoginButtonClicked()
{
	GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Green, "Login Button Clicked");

	FString ID = IdEditBox->GetText().ToString();
	FString Password = PasswordEditBox->GetText().ToString();

	//ID, PW�� 6�� ~ 20�� / ID���� Ư������ ���� X / PW���� ������ Ư������ ���� / ID, PW���� ���� ���� X
	if (IsValidID(ID) && IsValidPassword(Password))
	{
		IdEditBox->SetText(FText());
		PasswordEditBox->SetText(FText());
		GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Green, "Success Login");
		//��ϵ� id, pw���� Ȯ�� -> ����
	}
}

void ULoginWidgetBase::MembershipButtonClicked()
{
	GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Green, "Membership Button Clicked");

	//create accout widget ����
}

bool ULoginWidgetBase::IsValidID(const FString& ID)
{
	if (ID.Len() < 6 || ID.Len() > 20)
	{
		return false;
	}

	if (ID.Contains(TEXT(" ")))
	{
		return false;
	}

	FRegexPattern SpecialCharPattern(TEXT("[!@#$%^&*(),.?\":{}|<>]"));
	FRegexMatcher Matcher(SpecialCharPattern, ID);
	if (Matcher.FindNext())
	{
		return false;
	}

	return true;
}

bool ULoginWidgetBase::IsValidPassword(const FString& Password)
{
	if (Password.Len() < 6 || Password.Len() > 20)
	{
		return false;
	}

	if (Password.Contains(TEXT(" ")))
	{
		return false;
	}

	FRegexPattern SpecialCharPattern(TEXT("[!@#$%^&*(),.?\":{}|<>]"));
	FRegexMatcher Matcher(SpecialCharPattern, Password);
	if (!Matcher.FindNext())
	{
		return false;
	}

	return true;
}
