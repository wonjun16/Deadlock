// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemBase.h"
#include "Components/TimelineComponent.h"
#include "ItemFlashbang.generated.h"

/**
 * 
 */

class UCurveFloat;
class UCameraComponent;

UCLASS()
class DEADLOCK_API AItemFlashbang : public AItemBase
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

public:
	AItemFlashbang();

	virtual void ActivateAffect() override;

	UFUNCTION(NetMulticast, Reliable)
	void AffectedCharacter(ADeadlockCharacter* HitCharacter);
	void AffectedCharacter_Implementation(ADeadlockCharacter* HitCharacter);

	UFUNCTION()
	void UpdateBloomIntensityWeight(float Value);

	UFUNCTION()
	void FinishFlahbangEffect();

private:
	FTimeline FlashbangTimeline;

	UPROPERTY(EditAnywhere, Category = "Timeline")
	UCurveFloat* FlashbangCurve;

	UCameraComponent* AffectedCamera;
};
