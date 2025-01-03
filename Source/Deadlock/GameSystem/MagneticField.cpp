// Fill out your copyright notice in the Description page of Project Settings.


#include "MagneticField.h"
#include <Net/UnrealNetwork.h>
#include "../GameMode/DeadlockGameState.h"
#include <Deadlock/Player/DeadlockCharacter.h>



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

	bReplicates = true;

}

// Called when the game starts or when spawned
void AMagneticField::BeginPlay()
{
	Super::BeginPlay();
	//timer by funtion Name  지정
	UKismetSystemLibrary::K2_SetTimer(this, "SetHp", 0.5f, true);

	DeadlockGS = Cast<ADeadlockGameState>(UGameplayStatics::GetGameState(GetWorld()));
	// 타임라인에 곡선 데이터 추가
	FOnTimelineFloat ProgressFunction;
	ProgressFunction.BindUFunction(this, FName("UpdateScale"));
	UCurveFloat* NewCurve;
	// 커브 생성 및 설정
	NewCurve = NewObject<UCurveFloat>(this);  // UCurveFloat 객체 생성

	NewCurve->FloatCurve.AddKey(0.f, 600.0f);
	NewCurve->FloatCurve.AddKey(5.f, ChangeScales[0]);  
	NewCurve->FloatCurve.AddKey(10.f, ChangeScales[1]);  
	NewCurve->FloatCurve.AddKey(15.f, 0);  

	ScaleTimeline->AddInterpFloat(NewCurve, ProgressFunction);
	 
	FOnTimelineEvent OnTimelineFinishedEvent;
	OnTimelineFinishedEvent.BindUFunction(this, FName("OnTimelineFinished"));
	ScaleTimeline->SetTimelineFinishedFunc(OnTimelineFinishedEvent);
	// 타임라인 설정 및 시작
	ScaleTimeline->SetLooping(false);
	ScaleTimeline->SetPlayRate(1.0f);
	ScaleTimeline->SetTimelineLength(15.0f);
	TakeDamage = false;
}

void AMagneticField::OnRep_Random()
{
	FVector randomVec(RandomX, 0, RandomZ);
	SetActorLocation(randomVec);
}



// Called every frame
void AMagneticField::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 서버에서만 값을 갱신하도록 처리
	if (HasAuthority()) // 서버에서만 처리
	{
		DeadlockGS->RemainTime -= DeltaTime; // 시간 감소
	}

	CheckRemainTime(DeltaTime);
	// 타임라인 업데이트
	if (DeadlockGS->RemainTime <= 0)
	{
		if (!ScaleTimeline->IsPlaying())
			ScaleTimeline->Play();
	}
	else
	{
		if (ScaleTimeline->IsPlaying())
			ScaleTimeline->Stop();
	}
}

void AMagneticField::SetHp()
{

}

void AMagneticField::CheckRemainTime(float DeltaTime)
{
	
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
	ADeadlockHUD* MyHUD = Cast<ADeadlockHUD>(PlayerController->GetHUD());
	if (MyHUD)
	{
		if (DeadlockGS->RemainTime > 0)
		{
			// 분 계산: RemainTime을 60으로 나누고, 소수점 이하를 버림.
			int32 Minutes = FMath::FloorToInt(DeadlockGS->RemainTime / 60.0f);

			// 초 계산: RemainTime을 60으로 나눈 나머지 값을 소수점 이하를 버림.
			int32 Seconds = FMath::FloorToInt(FMath::Fmod(DeadlockGS->RemainTime, 60.0f));

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

	// 스케일 값을 업데이트
	FVector NewScale(Value, Value, Value);
	magneticMesh->SetWorldScale3D(NewScale);

	// 커브에서 시간 로그 출력 (ProgressFunction이 호출될 때마다 업데이트)
	if (ScaleTimeline)
	{
		// 타임라인의 현재 시간 확인
		float CurrentTime = ScaleTimeline->GetPlaybackPosition();  // 타임라인의 현재 시간
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, FString::Printf(TEXT("Current Time: %f, Scale Value: %f"), CurrentTime, Value));

		//첫번째 축소
		if (CurrentTime > 5.0f && !frist)
		{
			if (HasAuthority())
			{
				frist = true;
				DeadlockGS->RemainTime = 5.f;
			}
		}
		//두번째 축소
		else if (CurrentTime > 10.0f && !second)
		{
			if (HasAuthority())
			{
				second = true;
				DeadlockGS->RemainTime = 5.f;
			}

		}
	}
}

void AMagneticField::OnTimelineFinished()
{
}

void AMagneticField::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ADeadlockCharacter* DeadlockCharacter = Cast<ADeadlockCharacter>(OtherActor);
	if(DeadlockCharacter !=  NULL)
	DeadlockCharacter->TakeMagneticDamage = false;

}

void AMagneticField::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	ADeadlockCharacter* DeadlockCharacter = Cast<ADeadlockCharacter>(OtherActor);
	if (DeadlockCharacter != NULL)
	DeadlockCharacter->TakeMagneticDamage = true;

}

void AMagneticField::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AMagneticField, RandomX);
	DOREPLIFETIME(AMagneticField, RandomZ);
	DOREPLIFETIME(AMagneticField, RemainTime);
}