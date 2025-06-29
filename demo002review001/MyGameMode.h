// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MyGameMode.generated.h"

/**
 * 
 */
UCLASS()
class DEMO002REVIEW001_API AMyGameMode : public AGameModeBase
{
	GENERATED_BODY()
public:
    // 声明你要重写的函数
    virtual void BeginPlay() override;
};

