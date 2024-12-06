// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemBase.h"
#include "Components/StaticMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"
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
	ItemBaseEffect->SetupAttachment(ItemMesh);

	ItemParticle = CreateDefaultSubobject<UParticleSystem>("Particle");

	bReplicates = true;
	SetReplicateMovement(true);
	bIsCanBeDetroy = false;
}

void AItemBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AItemBase, EItemTypeIndex);
	DOREPLIFETIME(AItemBase, DamageAmount);
}

// Called when the game starts or when spawned
void AItemBase::BeginPlay()
{
	Super::BeginPlay();

	
	if (HasAuthority())
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Red, "Server Item BeginPlay");

		GetWorld()->GetTimerManager().SetTimer(ItemTriggerTimerHandle,
			this, &AItemBase::ServerItemAffect, ItemTimer, false);

		GetWorld()->GetTimerManager().SetTimer(ItemTriggerTimerHandle,
			this, &AItemBase::ServerItemEffect, ItemTimer, false);
	}
}

// Called every frame
void AItemBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AItemBase::ServerItemAffect_Implementation()
{
	ClientItemAffect();
	GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Red, "Server Item Affect");
}

void AItemBase::ClientItemAffect_Implementation()
{
	GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Red, "Client Item Affect");
}

void AItemBase::ServerItemEffect_Implementation()
{
	ClientItemEffect();
}

void AItemBase::ClientItemEffect_Implementation()
{
	if (HasAuthority())
	{
		if (ItemBaseEffect)
		{
			UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(),
				ItemBaseEffect->GetAsset(), ItemMesh->GetComponentLocation());
		}
		else if (ItemParticle)
		{
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(),
				ItemParticle, ItemMesh->GetComponentLocation());
		}
	}
}

void AItemBase::ServerEndItem_Implementation()
{
	ClientEndItem();
}

void AItemBase::ClientEndItem_Implementation()
{
	if (bIsCanBeDetroy)
	{
		this->Destroy();
	}
}
