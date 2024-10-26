// Fill out your copyright notice in the Description page of Project Settings.


#include "MagneticField.h"


// Sets default values
AMagneticField::AMagneticField()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	magneticMesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	magneticMesh->SetCollisionProfileName("OverlapAll");
	SetRootComponent(magneticMesh);
	magneticMesh->OnComponentBeginOverlap.AddDynamic(this, &AMagneticField::OnOverlapBegin);
	magneticMesh->OnComponentEndOverlap.AddDynamic(this, &AMagneticField::OnOverlapEnd);

	ScaleTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("ScaleTimeline"));

	bReplicates = true; // Actor가 복제되도록 설정

}

// Called when the game starts or when spawned
void AMagneticField::BeginPlay()
{
	Super::BeginPlay();
	//timer by funtion Name  지정
	UKismetSystemLibrary::K2_SetTimer(this, "SetHp", 0.5f, true);
	float RandomX = FMath::FRandRange(-10.0f, 10.0f);
	float RandomZ = FMath::FRandRange(-10.0f, 10.0f);
	FVector randomVec(RandomX, 0, RandomZ);
	SetActorLocation(randomVec);

	if (ScaleCurve)
	{
		// 타임라인에 곡선 데이터 추가
		FOnTimelineFloat ProgressFunction;
		ProgressFunction.BindUFunction(this, FName("UpdateScale"));
		ScaleTimeline->AddInterpFloat(ScaleCurve, ProgressFunction);

		// 타임라인 설정 및 시작
		ScaleTimeline->SetLooping(false);
		
	}
	
}

// Called every frame
void AMagneticField::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	CheckRemainTime(DeltaTime);
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::SanitizeFloat(RemainTime));
	// 타임라인 업데이트
	if (RemainTime <= 0)
	{
		if (!ScaleTimeline->IsPlaying())
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("OnUpdateScale"));
			ScaleTimeline->Play();
		}
		ScaleTimeline->TickComponent(DeltaTime, ELevelTick::LEVELTICK_TimeOnly, nullptr);
	}

}

void AMagneticField::SetHp()
{
	if (TakeDamage)
	{
		APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
		ADeadlockHUD* MyHUD = Cast<ADeadlockHUD>(PlayerController->GetHUD());
		if (MyHUD)
		{
			MyHUD->HeathUIInstance->CurHP -= 0.1f;
		}
	}
}

void AMagneticField::CheckRemainTime(float DeltaTime)
{
	RemainTime -= DeltaTime;
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
	ADeadlockHUD* MyHUD = Cast<ADeadlockHUD>(PlayerController->GetHUD());
	if (MyHUD)
	{
		if (RemainTime > 0)
		{
			// 분 계산: RemainTime을 60으로 나누고, 소수점 이하를 버림.
			int32 Minutes = FMath::FloorToInt(RemainTime / 60.0f);

			// 초 계산: RemainTime을 60으로 나눈 나머지 값을 소수점 이하를 버림.
			int32 Seconds = FMath::FloorToInt(FMath::Fmod(RemainTime, 60.0f));

			// "MM:SS" 형식의 텍스트를 생성
			FText FormattedTime = FText::Format(
				FText::FromString(TEXT("{0}:{1:02}")),
				FText::AsNumber(Minutes),
				FText::AsNumber(Seconds)
			);

			MyHUD->MagneticUIInstance-> Time = FormattedTime.ToString();
		}
		else
		{
			MyHUD->MagneticUIInstance->Time = "The magnetic field shrinks";
		}
		
	}
}

void AMagneticField::UpdateScale(float Value)
{

	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("UpdateScale"));
	// 스케일 값을 업데이트
	FVector NewScale = FMath::Lerp(FVector(1.0f, 1.0f, 1.0f), FVector(Value, Value, Value), Value);
	magneticMesh->SetWorldScale3D(NewScale);

	UE_LOG(LogTemp, Warning, TEXT("New Scale: %s"), *NewScale.ToString());

}

void AMagneticField::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("OnOverlapBegin"));
	TakeDamage = false;
}

void AMagneticField::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("OnOverlapEnd"));
	TakeDamage = true;
}

