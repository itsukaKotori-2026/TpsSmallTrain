// Copyright Epic Games, Inc. All Rights Reserved.

#include "firstTps003GameMode.h"
#include "firstTps003Character.h"
#include "UObject/ConstructorHelpers.h"

AfirstTps003GameMode::AfirstTps003GameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
