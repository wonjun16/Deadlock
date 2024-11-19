// Fill out your copyright notice in the Description page of Project Settings.


#include "AlertWidgetBase.h"
#include "Components/Button.h"

void UAlertWidgetBase::NativeConstruct()
{
	Super::NativeConstruct();

	OKButton = Cast<UButton>(GetWidgetFromName(TEXT("BP_OKButton")));

	if (OKButton)
	{
		OKButton->OnClicked.AddDynamic(this, &UAlertWidgetBase::OKButtonClicked);
	}
}

void UAlertWidgetBase::OKButtonClicked()
{
	SetVisibility(ESlateVisibility::Collapsed);
}
