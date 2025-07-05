// Copyright Epic Games, Inc. All Rights Reserved.

#include "demo003train005GameMode.h"
#include "demo003train005Character.h"
#include "UObject/ConstructorHelpers.h"

Ademo003train005GameMode::Ademo003train005GameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
