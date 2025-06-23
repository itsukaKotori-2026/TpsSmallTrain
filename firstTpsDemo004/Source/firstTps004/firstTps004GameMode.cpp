// Copyright Epic Games, Inc. All Rights Reserved.

#include "firstTps004GameMode.h"
#include "firstTps004Character.h"
#include "UObject/ConstructorHelpers.h"
#include "FirstTps004HUD.h"  // 加入你的HUD头文件
AfirstTps004GameMode::AfirstTps004GameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
	// 设置默认HUD类为自定义HUD
	HUDClass = AFirstTps004HUD::StaticClass();
}
