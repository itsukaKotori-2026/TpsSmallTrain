// Copyright Epic Games, Inc. All Rights Reserved.

#include "firstTps004GameMode.h"
#include "firstTps004Character.h"
#include "UObject/ConstructorHelpers.h"
#include "FirstTps004HUD.h"  // �������HUDͷ�ļ�
AfirstTps004GameMode::AfirstTps004GameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
	// ����Ĭ��HUD��Ϊ�Զ���HUD
	HUDClass = AFirstTps004HUD::StaticClass();
}
