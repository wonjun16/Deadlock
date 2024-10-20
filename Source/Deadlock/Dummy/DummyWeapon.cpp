// Fill out your copyright notice in the Description page of Project Settings.


#include "DummyWeapon.h"

// Sets default values
ADummyWeapon::ADummyWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ADummyWeapon::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ADummyWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

