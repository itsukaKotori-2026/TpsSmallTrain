// Copyright Epic Games, Inc. All Rights Reserved.

#include "demo007GameMode.h"
#include "demo007Character.h"
#include "UObject/ConstructorHelpers.h"

Ademo007GameMode::Ademo007GameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
