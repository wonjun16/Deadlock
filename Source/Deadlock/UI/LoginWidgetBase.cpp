// Fill out your copyright notice in the Description page of Project Settings.


#include "LoginWidgetBase.h"
#include "Components/EditableTextBox.h"
#include "Components/Button.h"
#include "regex"
#include "RegisterWidgetBase.h"
#include "AlertWidgetBase.h"
#include "../Client/ClientThread.h"

void ULoginWidgetBase::NativeConstruct()
{
	Super::NativeConstruct();

	IdEditBox = Cast<UEditableTextBox>(GetWidgetFromName(TEXT("BP_IdEditBox")));
	PasswordEditBox = Cast<UEditableTextBox>(GetWidgetFromName(TEXT("BP_PasswordEditBox")));
	LoginButton = Cast<UButton>(GetWidgetFromName(TEXT("BP_LoginButton")));
	MembershipButton = Cast<UButton>(GetWidgetFromName(TEXT("BP_MembershipButton")));
	RegisterWidget = Cast<URegisterWidgetBase>(GetWidgetFromName(TEXT("BP_RegisterWidget")));
	FailLoginWidget = Cast<UAlertWidgetBase>(GetWidgetFromName(TEXT("BP_FailLoginWidget")));

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
		LoginRequest->SetURL(TEXT("http://127.0.0.1:8080/login"));
		LoginRequest->SetContentAsString(JsonString);

		LoginRequest->OnProcessRequestComplete().BindUObject(this, &ULoginWidgetBase::OnResponseReceived);

		LoginRequest->ProcessRequest();
	}
	else
	{
		FailLoginWidget->SetVisibility(ESlateVisibility::Visible);
	}
}

void ULoginWidgetBase::MembershipButtonClicked()
{
	GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Green, "Membership Button Clicked");

	FailLoginWidget->SetVisibility(ESlateVisibility::Collapsed);

	//create accout widget 띄우기
	RegisterWidget->SetVisibility(ESlateVisibility::Visible);
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

void ULoginWidgetBase::OnResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	if (!bWasSuccessful || !Response.IsValid())
	{
		UE_LOG(LogTemp, Error, TEXT("HTTP request failed. URL: %s"), *Request->GetURL());
		return;
	}

	int32 StatusCode = Response->GetResponseCode();
	FString ResponseContent = Response->GetContentAsString();

	if (StatusCode == 200)
	{
		//로그인 성공 로직
		ClientThread* Thread = new ClientThread();
	}
	else
	{
		FailLoginWidget->SetVisibility(ESlateVisibility::Visible);
	}

	//UE_LOG(LogTemp, Warning, TEXT("HTTP request completed. Status Code: %d, URL: %s"), StatusCode, *Request->GetURL());
	//UE_LOG(LogTemp, Warning, TEXT("Response Content: %s"), *ResponseContent);
}
