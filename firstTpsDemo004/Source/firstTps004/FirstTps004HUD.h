// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "FirstTps004HUD.generated.h"

/**
 * 
 */
UCLASS()
class FIRSTTPS004_API AFirstTps004HUD : public AHUD
{
	GENERATED_BODY()

protected:
	virtual void DrawHUD() override;
	
};
