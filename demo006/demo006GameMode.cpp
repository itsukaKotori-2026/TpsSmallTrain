// Copyright Epic Games, Inc. All Rights Reserved.

#include "demo006GameMode.h"
#include "demo006Character.h"
#include "UObject/ConstructorHelpers.h"

Ademo006GameMode::Ademo006GameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
