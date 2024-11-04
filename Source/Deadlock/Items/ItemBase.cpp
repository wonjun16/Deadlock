// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemBase.h"
#include "Components/StaticMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"

// Sets default values
AItemBase::AItemBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>("ItemMesh");
	SetRootComponent(ItemMesh);
	ItemMesh->SetSimulatePhysics(true);
	ItemMesh->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	/* Do not Simulate Physics Before Grab
	*  Deactive During Test / Develop
	ItemMesh->SetSimulatePhysics(false); */
	

	FVector RootLocation = FVector(RootComponent->GetComponentLocation().X, RootComponent->GetComponentLocation().Y, RootComponent->GetComponentLocation().Z);

	CapsuleCollision = CreateDefaultSubobject<UCapsuleComponent>("CapsuleCollision");
	CapsuleCollision->SetupAttachment(RootComponent);

	ItemBaseEffect = CreateDefaultSubobject<UNiagaraComponent>("ItemEffect");
	ItemBaseEffect->SetupAttachment(RootComponent);
	EffectAsset = ItemBaseEffect->GetAsset();
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

void AItemBase::UseItem_Implementation(int currentitemcount)
{
	UE_LOG(LogTemp, Log, TEXT("UseItem Test Log"));
}

void AItemBase::GetItem_Implementation(int currentitemcount, int maxitemcount)
{

	UE_LOG(LogTemp, Log, TEXT("GetItem Test Log"));
}

void AItemBase::PlayItemEffect_Implementation()
{
	UNiagaraComponent* ItemEffect = UNiagaraFunctionLibrary::SpawnSystemAttached
	(EffectAsset, ItemMesh, FName("ItemMesh"), FVector(0.0f), FRotator(0.0f),
		EAttachLocation::Type::KeepRelativeOffset, true);
}

void AItemBase::ThrowMovement_Implementation(FVector ThrowDirection)
{
	UE_LOG(LogTemp, Log, TEXT("ThrowMovement Test Log"));
	//CapsuleCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	//Test Vector
	ItemMesh->AddImpulse(FVector(1700.0f, 0.0f, 1000.0f));
	//ItemMesh->AddImpulse(ThrowDirection);
}

void AItemBase::EventItemAffect_Implementation()
{
}
