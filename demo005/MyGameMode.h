// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MyGameMode.generated.h"

/**
 * 
 */
UCLASS()
class DEMO005_API AMyGameMode : public AGameModeBase
{
	GENERATED_BODY()
	

public:
	AMyGameMode(); // 必须声明构造函数
	virtual void BeginPlay() override;
};
