// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "MyHUD.generated.h"

/**
 * 
 */
UCLASS()
class DEMO008_API AMyHUD : public AHUD
{
	GENERATED_BODY()
protected:
	virtual void DrawHUD() override;
private:
	void DrawAmmo();
};
