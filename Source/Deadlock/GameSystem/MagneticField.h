// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Kismet/KismetSystemLibrary.h"
#include "MagneticField.generated.h"

UCLASS()
class DEADLOCK_API AMagneticField : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMagneticField();

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* magneticMesh;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool TakeDamage;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float RemainTime;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void SetHp();

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void CheckRemainTime(float DeltaTime);

};
