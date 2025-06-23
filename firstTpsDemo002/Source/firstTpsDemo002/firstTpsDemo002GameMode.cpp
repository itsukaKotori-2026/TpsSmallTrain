// Copyright Epic Games, Inc. All Rights Reserved.

#include "firstTpsDemo002GameMode.h"
#include "firstTpsDemo002Character.h"
#include "UObject/ConstructorHelpers.h"

AfirstTpsDemo002GameMode::AfirstTpsDemo002GameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
