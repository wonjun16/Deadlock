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
	DOREPLIFETIME(AItemBase, CurrentItemCount);
	DOREPLIFETIME(AItemBase, DamageAmount);
}

// Called when the game starts or when spawned
void AItemBase::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority())
	{
		GetWorld()->GetTimerManager().SetTimer(ItemTriggerTimerHandle,
			this, &AItemBase::EventItemAffect, ItemTimer, false);
	}
}

// Called every frame
void AItemBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AItemBase::PlayItemEffect_Implementation()
{
	if (HasAuthority())
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), ItemBaseEffect->GetAsset(), ItemMesh->GetComponentLocation());

		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ItemParticle, ItemMesh->GetComponentLocation());
		
	}
}

void AItemBase::ServerPlayEffect_Implementation()
{
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), ItemBaseEffect->GetAsset(), ItemMesh->GetComponentLocation());
	PlayItemEffect();
}


void AItemBase::EventItemAffect_Implementation()
{
	//ClientItemAffect();
	ServerPlayEffect();
}

void AItemBase::ClientItemAffect_Implementation()
{
	EventItemAffect();
	//ServerPlayEffect();
}

void AItemBase::EndItemEvent_Implementation()
{
	if (bIsCanBeDetroy)
	{
		this->Destroy();
	}
}
