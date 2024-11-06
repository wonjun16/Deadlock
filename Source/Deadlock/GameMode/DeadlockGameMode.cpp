// Copyright Epic Games, Inc. All Rights Reserved.

#include "DeadlockGameMode.h"
#include "../Player/DeadlockCharacter.h"
#include "UObject/ConstructorHelpers.h"



ADeadlockGameMode::ADeadlockGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}

AActor* ADeadlockGameMode::ChoosePlayerStart_Implementation(AController* Player)
{
	return nullptr;
}
