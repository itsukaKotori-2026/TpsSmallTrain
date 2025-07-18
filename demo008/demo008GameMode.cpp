// Copyright Epic Games, Inc. All Rights Reserved.

#include "demo008GameMode.h"
#include "demo008Character.h"
#include "UObject/ConstructorHelpers.h"

Ademo008GameMode::Ademo008GameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
