// Fill out your copyright notice in the Description page of Project Settings.


#include "Bullet.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Net/UnrealNetwork.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"

// Sets default values
ABullet::ABullet()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BulletMesh = CreateDefaultSubobject<UStaticMeshComponent>("Bullet");
	RootComponent = BulletMesh;
	BulletMesh->SetCollisionProfileName(TEXT("Bullet"));
	BulletMesh->AddWorldRotation(FRotator(0, -90, 0));

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>("Movement");
	ProjectileMovement->InitialSpeed = 1000.0f;
	ProjectileMovement->MaxSpeed = 1000.0f;
	ProjectileMovement->bRotationFollowsVelocity = false;
	ProjectileMovement->bShouldBounce = false;

	bReplicates = true;
	SetReplicateMovement(true);
}

void ABullet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ABullet, OwnerCharacter);
	DOREPLIFETIME(ABullet, Damage);
}

// Called when the game starts or when spawned
void ABullet::BeginPlay()
{
	Super::BeginPlay();
}

void ABullet::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	AddDamage(OtherActor);
}

// Called every frame
void ABullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABullet::Fire(const FVector& Direction)
{
	ProjectileMovement->Velocity = Direction * ProjectileMovement->InitialSpeed;
	GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Green, FString::Printf(TEXT("Damage : %f"), Damage));
}

void ABullet::AddDamage_Implementation(AActor* OtherActor)
{
	TObjectPtr<ACharacter> OverlapCharacter = Cast<ACharacter>(OtherActor);
	if (OverlapCharacter && OverlapCharacter != OwnerCharacter)
	{
		//add Damage to overlap character
		UGameplayStatics::ApplyDamage(OverlapCharacter, Damage, OwnerCharacter->GetController(), this, 0);
		GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Green,"Success Damage");
		Destroy();
	}
}

