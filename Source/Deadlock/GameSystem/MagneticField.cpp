// Fill out your copyright notice in the Description page of Project Settings.


#include "MagneticField.h"
#include <Net/UnrealNetwork.h>



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

	bReplicates = true; // Actor�� �����ǵ��� ����

}

// Called when the game starts or when spawned
void AMagneticField::BeginPlay()
{
	Super::BeginPlay();
	//timer by funtion Name  ����
	UKismetSystemLibrary::K2_SetTimer(this, "SetHp", 0.5f, true);

//	GEngine->AddOnScreenDebugMessage(-1, 60.f, FColor::Red, TEXT("Client - RandomX and RandomZ updated"));

	if (HasAuthority())
	{
		RandomX = FMath::FRandRange(-10.0f, 10.0f);
		RandomZ = FMath::FRandRange(-10.0f, 10.0f);
		FVector randomVec(RandomX, 0, RandomZ);
		SetActorLocation(randomVec);
	}

	//if (ScaleCurve)
	{
		// Ÿ�Ӷ��ο� � ������ �߰�
		FOnTimelineFloat ProgressFunction;
		ProgressFunction.BindUFunction(this, FName("UpdateScale"));
		UCurveFloat* NewCurve;
		// Ŀ�� ���� �� ����
		NewCurve = NewObject<UCurveFloat>(this);  // UCurveFloat ��ü ����

		NewCurve->FloatCurve.AddKey(0.f, 600.0f);
		NewCurve->FloatCurve.AddKey(5.f, ChangeScales[0]);  
		NewCurve->FloatCurve.AddKey(10.f, ChangeScales[1]);  
		NewCurve->FloatCurve.AddKey(15.f, 0);  

		ScaleTimeline->AddInterpFloat(NewCurve, ProgressFunction);

		 
		FOnTimelineEvent OnTimelineFinishedEvent;
		OnTimelineFinishedEvent.BindUFunction(this, FName("OnTimelineFinished"));
		ScaleTimeline->SetTimelineFinishedFunc(OnTimelineFinishedEvent);
		// Ÿ�Ӷ��� ���� �� ����
		ScaleTimeline->SetLooping(false);
		ScaleTimeline->SetPlayRate(1.0f);
		ScaleTimeline->SetTimelineLength(15.0f);
		//ScaleTimeline->PlayFromStart();
	}
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

	// ���������� ���� �����ϵ��� ó��
	if (HasAuthority()) // ���������� ó��
	{
		RemainTime -= DeltaTime; // �ð� ����
	}

	CheckRemainTime(DeltaTime);
	// Ÿ�Ӷ��� ������Ʈ
	if (RemainTime <= 0)
	{
		if (!ScaleTimeline->IsPlaying())
			ScaleTimeline->Play();
		//ScaleTimeline->TickComponent(DeltaTime, ELevelTick::LEVELTICK_TimeOnly, nullptr);
	}
	else
	{
		if (ScaleTimeline->IsPlaying())
			ScaleTimeline->Stop();
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
	
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
	ADeadlockHUD* MyHUD = Cast<ADeadlockHUD>(PlayerController->GetHUD());
	if (MyHUD)
	{
		if (RemainTime > 0)
		{
			// �� ���: RemainTime�� 60���� ������, �Ҽ��� ���ϸ� ����.
			int32 Minutes = FMath::FloorToInt(RemainTime / 60.0f);

			// �� ���: RemainTime�� 60���� ���� ������ ���� �Ҽ��� ���ϸ� ����.
			int32 Seconds = FMath::FloorToInt(FMath::Fmod(RemainTime, 60.0f));

			// "MM:SS" ������ �ؽ�Ʈ�� ����
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

	// ������ ���� ������Ʈ
	//FVector NewScale = FMath::Lerp(FVector(1.0f, 1.0f, 1.0f), FVector(Value, Value, Value), Value);
	FVector NewScale(Value, Value, Value);
	magneticMesh->SetWorldScale3D(NewScale);

	// Ŀ�꿡�� �ð� �α� ��� (ProgressFunction�� ȣ��� ������ ������Ʈ)
	if (ScaleTimeline)
	{
		// Ÿ�Ӷ����� ���� �ð� Ȯ��
		float CurrentTime = ScaleTimeline->GetPlaybackPosition();  // Ÿ�Ӷ����� ���� �ð�
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, FString::Printf(TEXT("Current Time: %f, Scale Value: %f"), CurrentTime, Value));

		//ù��° ���
		if (CurrentTime > 5.0f && CurrentTime < 5.01f && RemainTime <= 0)
		{
			GEngine->AddOnScreenDebugMessage(-1, 60.f, FColor::Red, TEXT("555555555555"));
			if (HasAuthority())
			{
				RemainTime = 5.f;
			}
		}
		//�ι�° ���
		else if (CurrentTime > 10.0f && CurrentTime < 10.01f && RemainTime <= 0)
		{
			GEngine->AddOnScreenDebugMessage(-1, 60.f, FColor::Red, TEXT("555555555555"));
			if (HasAuthority())
			{
				RemainTime = 5.f;
			}

		}
	}
}

void AMagneticField::OnTimelineFinished()
{
	
	GEngine->AddOnScreenDebugMessage(-1, 60.f, FColor::Red, TEXT("OnTimelineFinished"));
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

void AMagneticField::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AMagneticField, RandomX);
	DOREPLIFETIME(AMagneticField, RandomZ);
	DOREPLIFETIME(AMagneticField, RemainTime);
}