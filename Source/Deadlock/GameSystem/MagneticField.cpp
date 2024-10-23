// Fill out your copyright notice in the Description page of Project Settings.


#include "MagneticField.h"

// Sets default values
AMagneticField::AMagneticField()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	magneticMesh = CreateDefaultSubobject<UStaticMeshComponent>("Weapon");
	SetRootComponent(magneticMesh);
	bReplicates = true; // Actor가 복제되도록 설정

}

// Called when the game starts or when spawned
void AMagneticField::BeginPlay()
{
	Super::BeginPlay();

	UKismetSystemLibrary::K2_SetTimer(this, "SetHp", 2.0f, true);
	//FVector randomVec = 
	//SetActorLocation()
	
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
}

