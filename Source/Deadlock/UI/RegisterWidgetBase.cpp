// Fill out your copyright notice in the Description page of Project Settings.


#include "RegisterWidgetBase.h"
#include "Components/EditableTextBox.h"
#include "Components/Button.h"
#include "regex"
#include "AlertWidgetBase.h"
#include "Blueprint/WidgetLayoutLibrary.h"

void URegisterWidgetBase::NativeConstruct()
{
	Super::NativeConstruct();

	IdEditBox = Cast<UEditableTextBox>(GetWidgetFromName(TEXT("BP_IdEditBox")));
	PasswordEditBox = Cast<UEditableTextBox>(GetWidgetFromName(TEXT("BP_PasswordEditBox")));
	NickNameEditBox = Cast<UEditableTextBox>(GetWidgetFromName(TEXT("BP_NickNameEditBox")));
	RegisterButton = Cast<UButton>(GetWidgetFromName(TEXT("BP_RegisterButton")));
	GoBackButton = Cast<UButton>(GetWidgetFromName(TEXT("BP_GoBackButton")));
	SuccessRegisterWidget = Cast<UAlertWidgetBase>(GetWidgetFromName(TEXT("BP_SuccessRegisterWidget")));
	FailRegisterWidget = Cast<UAlertWidgetBase>(GetWidgetFromName(TEXT("BP_FailRegisterWidget")));

	if (RegisterButton)
	{
		RegisterButton->OnClicked.AddDynamic(this, &URegisterWidgetBase::RegisterButtonClicked);
	}
	if (GoBackButton)
	{
		GoBackButton->OnClicked.AddDynamic(this, &URegisterWidgetBase::GoBackButtonClicked);
	}
}

void URegisterWidgetBase::RegisterButtonClicked()
{
	GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Green, "Register Button Clicked");

	FString ID = IdEditBox->GetText().ToString();
	FString Password = PasswordEditBox->GetText().ToString();
	FString Nickname = NickNameEditBox->GetText().ToString();

	if (IsValidID(ID) && IsValidPassword(Password) && IsValidNickName(Nickname))
	{
		IdEditBox->SetText(FText());
		PasswordEditBox->SetText(FText());
		NickNameEditBox->SetText(FText());
		GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Green, "Success Create Account");

		//юс╫ц req
		TSharedPtr<FJsonObject> RegisterMessage = MakeShareable(new FJsonObject);
		RegisterMessage->SetStringField("id", ID);
		RegisterMessage->SetStringField("password", Password);
		RegisterMessage->SetStringField("nickname", Nickname);

		FString JsonString;
		TSharedRef<TJsonWriter<TCHAR>> JsonWriter = TJsonWriterFactory<>::Create(&JsonString);
		FJsonSerializer::Serialize(RegisterMessage.ToSharedRef(), JsonWriter);

		TSharedRef<IHttpRequest> RegisterRequest = FHttpModule::Get().CreateRequest();
		RegisterRequest->SetVerb(TEXT("POST"));
		RegisterRequest->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
		RegisterRequest->SetURL(TEXT("http://192.168.3.55:8080/register"));
		RegisterRequest->SetContentAsString(JsonString);

		RegisterRequest->OnProcessRequestComplete().BindUObject(this, &URegisterWidgetBase::OnResponseReceived);

		RegisterRequest->ProcessRequest();
	}
	else
	{
		FailRegisterWidget->SetVisibility(ESlateVisibility::Visible);
	}
}

void URegisterWidgetBase::GoBackButtonClicked()
{
	IdEditBox->SetText(FText());
	PasswordEditBox->SetText(FText());
	NickNameEditBox->SetText(FText());

	SuccessRegisterWidget->SetVisibility(ESlateVisibility::Collapsed);
	FailRegisterWidget->SetVisibility(ESlateVisibility::Collapsed);

	SetVisibility(ESlateVisibility::Collapsed);
}

bool URegisterWidgetBase::IsValidID(const FString& ID)
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

bool URegisterWidgetBase::IsValidPassword(const FString& Password)
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

bool URegisterWidgetBase::IsValidNickName(const FString& NickName)
{
	if (NickName.Len() < 3 || NickName.Len() > 12)
	{
		return false;
	}

	if (NickName.Contains(TEXT(" ")))
	{
		return false;
	}

	FRegexPattern SpecialCharPattern(TEXT("[!@#$%^&*(),.?\":{}|<>]"));
	FRegexMatcher Matcher(SpecialCharPattern, NickName);
	if (Matcher.FindNext())
	{
		return false;
	}

	return true;
}

void URegisterWidgetBase::OnResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	if (!bWasSuccessful || !Response.IsValid())
	{
		UE_LOG(LogTemp, Error, TEXT("HTTP request failed. URL: %s"), *Request->GetURL());
		return;
	}

	int32 StatusCode = Response->GetResponseCode();
	FString ResponseContent = Response->GetContentAsString();

	if (StatusCode == 201)
	{
		SuccessRegisterWidget->SetVisibility(ESlateVisibility::Visible);
		UE_LOG(LogTemp, Warning, TEXT("HTTP request completed. Status Code: %d, URL: %s"), StatusCode, *Request->GetURL());
	}
	else
	{
		FailRegisterWidget->SetVisibility(ESlateVisibility::Visible);
		UE_LOG(LogTemp, Warning, TEXT("HTTP request completed. Status Code: %d, URL: %s"), StatusCode, *Request->GetURL());
	}
	/*UE_LOG(LogTemp, Warning, TEXT("HTTP request completed. Status Code: %d, URL: %s"), StatusCode, *Request->GetURL());
	UE_LOG(LogTemp, Warning, TEXT("Response Content: %s"), *ResponseContent);*/
}
