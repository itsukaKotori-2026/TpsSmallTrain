// Copyright Epic Games, Inc. All Rights Reserved.

#include "demo002review001GameMode.h"
#include "demo002review001Character.h"
#include "UObject/ConstructorHelpers.h"

Ademo002review001GameMode::Ademo002review001GameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
