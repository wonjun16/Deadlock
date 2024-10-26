// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DeadlockHUD.h"
#include "Components/TimelineComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
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

	// 타임라인 컴포넌트
	UPROPERTY()
	UTimelineComponent* ScaleTimeline;

	// 타임라인에 사용할 곡선
	UPROPERTY(EditAnywhere, Category = "Timeline")
	UCurveFloat* ScaleCurve;

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

	// 타임라인의 값을 업데이트하는 함수
	UFUNCTION()
	void UpdateScale(float Value);

	// 겹침 시작 시 호출되는 함수
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	// 겹침 종료 시 호출되는 함수
	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor ,UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);


};
