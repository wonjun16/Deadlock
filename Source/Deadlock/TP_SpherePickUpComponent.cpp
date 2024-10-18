// Fill out your copyright notice in the Description page of Project Settings.


#include "TP_SpherePickUpComponent.h"

UTP_SpherePickUpComponent::UTP_SpherePickUpComponent()
{
	// Setup the Sphere Collision
	SphereRadius = 32.f;
}

void UTP_SpherePickUpComponent::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(LogTemp, Log, TEXT("Log Message"));
	// Register our Overlap Event
	OnComponentBeginOverlap.AddDynamic(this, &UTP_SpherePickUpComponent::OnSphereBeginOverlap);

}

void UTP_SpherePickUpComponent::OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Log, TEXT("Overlapped"));
	// Checking if it is a First Person Character overlapping
	ADeadlockCharacter* Character = Cast<ADeadlockCharacter>(OtherActor);
	if (Character != nullptr)
	{
		UE_LOG(LogTemp, Log, TEXT("ADeadlockCharacter"));
		// Notify that the actor is being picked up
		//OnPickUp.Broadcast(Character);
		AttachToComponent(Character->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName("weapon"));

		// Unregister from the Overlap Event so it is no longer triggered
		OnComponentBeginOverlap.RemoveAll(this);
	}
}
