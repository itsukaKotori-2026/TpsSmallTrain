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

    // ��ȡ��Ļ����λ��
    const float CenterX = Canvas->ClipX * 0.5f;
    const float CenterY = Canvas->ClipY * 0.5f;

    const float CrosshairSize = 10.0f;

    // ����Ļ���Ļ���һ����ɫʮ��׼��
    DrawLine(CenterX - CrosshairSize, CenterY, CenterX + CrosshairSize, CenterY, FLinearColor::White);
    DrawLine(CenterX, CenterY - CrosshairSize, CenterX, CenterY + CrosshairSize, FLinearColor::White);
}

