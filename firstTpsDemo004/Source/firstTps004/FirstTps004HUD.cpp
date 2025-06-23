// Fill out your copyright notice in the Description page of Project Settings.


#include "FirstTps004HUD.h"
#include "Engine/Canvas.h"
#include "Engine/Engine.h"

void AFirstTps004HUD::DrawHUD()
{
    Super::DrawHUD();

    if (Canvas == nullptr)
    {
        return;
    }

    // 获取屏幕中心位置
    const float CenterX = Canvas->ClipX * 0.5f;
    const float CenterY = Canvas->ClipY * 0.5f;

    const float CrosshairSize = 10.0f;

    // 在屏幕中心绘制一个白色十字准星
    DrawLine(CenterX - CrosshairSize, CenterY, CenterX + CrosshairSize, CenterY, FLinearColor::White);
    DrawLine(CenterX, CenterY - CrosshairSize, CenterX, CenterY + CrosshairSize, FLinearColor::White);
}

