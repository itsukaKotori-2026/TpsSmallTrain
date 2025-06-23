// Copyright Epic Games, Inc. All Rights Reserved.

#include "firstTps002GameMode.h"
#include "firstTps002Character.h"
#include "UObject/ConstructorHelpers.h"

AfirstTps002GameMode::AfirstTps002GameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
