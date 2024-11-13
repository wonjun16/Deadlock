// Fill out your copyright notice in the Description page of Project Settings.


#include "Spawner.h"
#include "Kismet/GameplayStatics.h"
#include "../Player/DeadlockCharacter.h"
#include "../GameMode/DeadlockPlayerController.h"
#include "GameFramework/PlayerController.h"
#include <Net/UnrealNetwork.h>
#include "Engine/World.h"

// Sets default values
ASpawner::ASpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


}

// Called when the game starts or when spawned
void ASpawner::BeginPlay()
{
	Super::BeginPlay();

	int32 PlayerIndex = 0;
	TArray<AActor*> FoundActors;

	ADeadlockCharacter* DeadlockCharacter = Cast<ADeadlockCharacter>(UGameplayStatics::GetActorOfClass(GetWorld(), ADeadlockCharacter::StaticClass()));
	DeadlockCharacter->S2CSetCharacterLocation(SpawnLocations);
}

// Called every frame
void ASpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASpawner::S2CSetCharacterLocation_Implementation()
{
	TArray<AActor*> PlayerCharacter;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ADeadlockCharacter::StaticClass(), PlayerCharacter);

	//if (HasAuthority())  // 서버에서만
	//{
	//	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("server is  : %d"), PlayerCharacter.Num()));
	//}
	//else  // 클라이언트에서만
	//{
	//	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, FString::Printf(TEXT("client is  : %d"), PlayerCharacter.Num()));
	//}


	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("PlayerCharacter is  : %d"), PlayerCharacter.Num()));
}

void ASpawner::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// SpawnLocations 배열이 네트워크에서 동기화되도록 설정
	DOREPLIFETIME(ASpawner, SpawnLocations);
	DOREPLIFETIME(ASpawner, SpawnIndex);
}

