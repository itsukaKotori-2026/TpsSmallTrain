// Copyright Epic Games, Inc. All Rights Reserved.

#include "demo003review003GameMode.h"
#include "demo003review003Character.h"
#include "UObject/ConstructorHelpers.h"
#include "MyHUD.h"
// ���Զ��� GameMode �Ĺ��캯��������
Ademo003review003GameMode::Ademo003review003GameMode()
{

	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}


}
