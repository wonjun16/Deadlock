// Fill out your copyright notice in the Description page of Project Settings.


#include "TitlePlayerController.h"
#include "../UI/LoginWidgetBase.h"

void ATitlePlayerController::BeginPlay()
{
	Super::BeginPlay();

	FSoftClassPath LoginWidgetClassPath(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/Deadlock/Widget/WBP_Title.WBP_Title_C'"));

	if (IsLocalController())
	{
		UClass* LoginWidgetClass = LoginWidgetClassPath.TryLoadClass<ULoginWidgetBase>();

		if (IsValid(LoginWidgetClass))
		{
			LoginWidget = CreateWidget<ULoginWidgetBase>(this, LoginWidgetClass);

			if (IsValid(LoginWidget))
			{
				LoginWidget->AddToViewport();
				SetInputMode(FInputModeGameAndUI());
				SetShowMouseCursor(true);
			}
		}
	}
}
