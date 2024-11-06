// Fill out your copyright notice in the Description page of Project Settings.


#include "Spawner.h"
#include "Kismet/GameplayStatics.h"
#include "../Player/DeadlockCharacter.h"
#include <Net/UnrealNetwork.h>

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

	//int32 PlayerIndex = 0;
	TArray<AActor*> PlayerCharacter;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ADeadlockCharacter::StaticClass(), PlayerCharacter);

	for (AActor* Actor : PlayerCharacter)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Actor Type: ") + Actor->GetClass()->GetName());

		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, FString::Printf(TEXT("MyCharacter spawned with ID: %s"), *GetName()));
		FVector RandomLocation = FVector(FMath::RandRange(-500.0f, 500.0f), FMath::RandRange(-500.0f, 500.0f), 300.0f);


		ADeadlockCharacter* DeadlockCharacter = Cast<ADeadlockCharacter>(Actor);
		if (DeadlockCharacter)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, FString::Printf(TEXT("SpawnIndex is  : %d"), SpawnIndex));
			if (SpawnIndex == 0)
			{
				GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, FString::Printf(TEXT("aaaaaaaaa")));
				DeadlockCharacter->S2CSetCharacterLocation(FVector(100.0f, -200.0f, 0.0f));
			}
			else if (SpawnIndex == 1)
			{
				GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, FString::Printf(TEXT("bbbbbbbbbbbbbb")));
				DeadlockCharacter->S2CSetCharacterLocation(FVector(300.0f, -400.0f, 0.0f));
			}
			//DeadlockCharacter->S2CSetCharacterLocation(SpawnLocations[PlayerIndex]);
			SpawnIndex++;
		}
	
	}
	
}

// Called every frame
void ASpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASpawner::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// SpawnLocations 배열이 네트워크에서 동기화되도록 설정
	DOREPLIFETIME(ASpawner, SpawnLocations);
	DOREPLIFETIME(ASpawner, SpawnIndex);
}

