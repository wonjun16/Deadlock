// Fill out your copyright notice in the Description page of Project Settings.


#include "LoginWidgetBase.h"
#include "Components/EditableTextBox.h"
#include "Components/Button.h"
#include "regex"
#include "Http.h"

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

	//ID, PW는 6자 ~ 20자 / ID에는 특수문자 포함 X / PW에는 무조건 특수문자 포함 / ID, PW에는 공백 포함 X
	if (IsValidID(ID) && IsValidPassword(Password))
	{
		IdEditBox->SetText(FText());
		PasswordEditBox->SetText(FText());
		GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Green, "Success Login");

		//임시 http req
		TSharedPtr<FJsonObject> LoginMessage = MakeShareable(new FJsonObject);
		LoginMessage->SetStringField("id", ID);
		LoginMessage->SetStringField("password", Password);

		FString JsonString;
		TSharedRef<TJsonWriter<TCHAR>> JsonWriter = TJsonWriterFactory<>::Create(&JsonString);
		FJsonSerializer::Serialize(LoginMessage.ToSharedRef(), JsonWriter);

		TSharedRef<IHttpRequest> LoginRequest = FHttpModule::Get().CreateRequest();
		LoginRequest->SetVerb(TEXT("POST"));
		LoginRequest->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
		LoginRequest->SetURL(TEXT("https://127.0.0.1:8080/login"));
		LoginRequest->SetContentAsString(JsonString);

		//Request->OnProcessRequestComplete().BindUObject(this, &ATestProjectGameModeBase::OnResponseReceived);

		LoginRequest->ProcessRequest();
	}
}

void ULoginWidgetBase::MembershipButtonClicked()
{
	GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Green, "Membership Button Clicked");

	//create accout widget 띄우기
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
