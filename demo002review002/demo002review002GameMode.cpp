// Copyright Epic Games, Inc. All Rights Reserved.

#include "demo002review002GameMode.h"
#include "demo002review002Character.h"
#include "UObject/ConstructorHelpers.h"

Ademo002review002GameMode::Ademo002review002GameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
