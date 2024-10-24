// Fill out your copyright notice in the Description page of Project Settings.


#include "MagneticField.h"
#include "GameMode/DeadlockHUD.h"

// Sets default values
AMagneticField::AMagneticField()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	magneticMesh = CreateDefaultSubobject<UStaticMeshComponent>("Weapon");
	SetRootComponent(magneticMesh);
	bReplicates = true; // Actor�� �����ǵ��� ����

}

// Called when the game starts or when spawned
void AMagneticField::BeginPlay()
{
	Super::BeginPlay();

	UKismetSystemLibrary::K2_SetTimer(this, "SetHp", 2.0f, true);
	float RandomX = FMath::FRandRange(-10.0f, 10.0f);
	float RandomZ = FMath::FRandRange(-10.0f, 10.0f);
	FVector randomVec(RandomX, 0, RandomZ);
	SetActorLocation(randomVec);
	
}

// Called every frame
void AMagneticField::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMagneticField::SetHp()
{
}

void AMagneticField::CheckRemainTime(float DeltaTime)
{
	RemainTime -= DeltaTime;
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
	ADeadlockHUD* MyHUD = Cast<ADeadlockHUD>(PlayerController->GetHUD());
	if (MyHUD)
	{
		// HUD�� ����� �� �ִ� �ڵ� �߰�
	}
}

