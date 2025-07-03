// Copyright Epic Games, Inc. All Rights Reserved.

#include "demo005GameMode.h"
#include "demo005Character.h"
#include "UObject/ConstructorHelpers.h"

Ademo005GameMode::Ademo005GameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
