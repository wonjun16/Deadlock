// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "AlertWidgetBase.generated.h"

class UButton;

/**
 * 
 */
UCLASS()
class DEADLOCK_API UAlertWidgetBase : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UButton> OKButton;

	UFUNCTION()
	void OKButtonClicked();
};
