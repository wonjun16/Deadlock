// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemBase.h"
#include "Components/StaticMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Deadlock/Player/DeadlockCharacter.h"
#include "Net/UnrealNetwork.h"
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

	ItemMesh->SetSimulatePhysics(false);

	ItemBaseEffect = CreateDefaultSubobject<UNiagaraComponent>("ItemEffect");
	ItemBaseEffect->SetupAttachment(RootComponent);

	bReplicates = true;
	SetReplicateMovement(true);
}

void AItemBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AItemBase, EItemTypeIndex);
	DOREPLIFETIME(AItemBase, CurrentItemCount);
	DOREPLIFETIME(AItemBase, DamageAmount);
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

EItemType AItemBase::GetItem_Implementation()
{
	return EItemType(EItemTypeIndex);
}

void AItemBase::PlayItemEffect_Implementation()
{
	if (HasAuthority())
	{
		UNiagaraComponent* ItemEffect = UNiagaraFunctionLibrary::SpawnSystemAttached
		(ItemBaseEffect->GetAsset(), ItemMesh, FName("ItemMesh"), FVector(0.0f), FRotator(0.0f),
			EAttachLocation::Type::KeepRelativeOffset, true);
		GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Red, "Server Effect");
	}
}

void AItemBase::ServerPlayEffect_Implementation()
{
	PlayItemEffect();
}

void AItemBase::ThrowMovement_Implementation(FVector ThrowDirection)
{
	//Simulate Physics When Throw
	ItemMesh->SetSimulatePhysics(true);

	CapsuleCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	FVector ThrowVector(ThrowDirection.X, ThrowDirection.Y, 1.0f);

	ItemMesh->AddImpulse(ThrowVector.GetSafeNormal() * 700, NAME_None, true);

	Execute_StartItemTimer(this);
}

void AItemBase::EventItemAffect_Implementation()
{
	PlayItemEffect();
}

void AItemBase::StartItemTimer_Implementation()
{
}

void AItemBase::EndItemEvent_Implementation()
{
	this->Destroy();
}
