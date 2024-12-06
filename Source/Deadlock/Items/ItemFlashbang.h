// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemBase.h"
#include "Components/TimelineComponent.h"
#include "ItemFlashbang.generated.h"

/**
 * 
 */
UCLASS()
class DEADLOCK_API AItemFlashbang : public AItemBase
{
	GENERATED_BODY()

public:
	AItemFlashbang();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UTimelineComponent* FlashTimeline;

	UPROPERTY(EditAnywhere)
	UCurveFloat* FlashTimelineFloatCurve;

	FOnTimelineFloat UpdateFlashFloat;

	virtual void ActivateAffect() override;

};
