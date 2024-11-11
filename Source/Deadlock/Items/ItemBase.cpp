// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemBase.h"
#include "Components/StaticMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Deadlock/Player/DeadlockCharacter.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"

// Sets default values
AItemBase::AItemBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CapsuleCollision = CreateDefaultSubobject<UCapsuleComponent>("CapsuleCollision");
	SetRootComponent(CapsuleCollision);

	ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>("ItemMesh");
	ItemMesh->SetupAttachment(RootComponent);

	ItemMesh->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	/* Do not Simulate Physics Before Grab
	*  Deactive During Test / Develop */
	//ItemMesh->SetSimulatePhysics(false);

	ItemBaseEffect = CreateDefaultSubobject<UNiagaraComponent>("ItemEffect");
	ItemBaseEffect->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AItemBase::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AItemBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AItemBase::UseItem_Implementation(int CurrentCount)
{
	UE_LOG(LogTemp, Log, TEXT("UseItem Test Log"));
	if (CurrentCount > 0)
	{
		CurrentItemCount = CurrentCount--;
		ItemMesh->SetSimulatePhysics(true);
	}
}

void AItemBase::GetItem_Implementation(int CurrentCount, int MaxCount)
{
	UE_LOG(LogTemp, Log, TEXT("GetItem Test Log"));
	if (MaxCount > CurrentCount)
	{
		CurrentItemCount = CurrentCount++;

		AActor::Destroy();
	}
}

void AItemBase::PlayItemEffect_Implementation()
{
	UNiagaraComponent* ItemEffect = UNiagaraFunctionLibrary::SpawnSystemAttached
	(ItemBaseEffect->GetAsset(), ItemMesh, FName("ItemMesh"), FVector(0.0f), FRotator(0.0f),
		EAttachLocation::Type::KeepRelativeOffset, true);

	ItemEffect->Activate();
}

void AItemBase::ThrowMovement_Implementation(FVector ThrowDirection)
{
	//Simulate Physics When Throw
	ItemMesh->SetSimulatePhysics(true);
	UE_LOG(LogTemp, Log, TEXT("ThrowMovement Test Log"));
	//CapsuleCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	//Test Vector
	ItemMesh->AddImpulse(FVector(1700.0f, 0.0f, 1000.0f));
	//ItemMesh->AddImpulse(ThrowDirection);
}

void AItemBase::EventItemAffect_Implementation()
{
}
