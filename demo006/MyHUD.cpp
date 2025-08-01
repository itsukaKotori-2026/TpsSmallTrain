// Fill out your copyright notice in the Description page of Project Settings.


#include "MyHUD.h"

#include "demo006Character.h"
#include "Engine/Canvas.h"
#include "Engine/Engine.h"

void AMyHUD::DrawHUD()
{
    Super::DrawHUD();
    // 在这里调用 DrawAmmo 方法
    DrawAmmo();
    if (Canvas == nullptr)
    {
        return;
    }

    // 获取屏幕中心位置
    const float CenterX = Canvas->ClipX * 0.5f;
    const float CenterY = Canvas->ClipY * 0.5f;

    //const float CrosshairSize = 10.0f;

    //// 在屏幕中心绘制一个白色十字准星
    //DrawLine(CenterX - CrosshairSize, CenterY, CenterX + CrosshairSize, CenterY, FLinearColor::White);
    //DrawLine(CenterX, CenterY - CrosshairSize, CenterX, CenterY + CrosshairSize, FLinearColor::White);
    float Spread = 5.0f + Character->CrosshairSpread;

    DrawLine(CenterX - Spread, CenterY, CenterX + Spread, CenterY, FLinearColor::White);
    DrawLine(CenterX, CenterY - Spread, CenterX, CenterY + Spread, FLinearColor::White);
}
void AMyHUD::DrawAmmo()
{

    Ademo006Character* Character = Cast<Ademo006Character>(GetOwningPawn());
    if (!Character) return;

    AAWeapon* Weapon = Character->GetEquippedWeapon();
    if (!Weapon) return;

    int32 Current = Weapon->GetCurrentAmmo();
    UE_LOG(LogTemp, Warning, TEXT("Current Ammo当前子弹数为: %d"), Current);  // 调试输出
    int32 Total = Weapon->GetTotalAmmo();

    FString AmmoText = FString::Printf(TEXT("Ammo: %d / %d"), Current, Total);
    float ScreenX = Canvas->ClipX - 200.f;
    float ScreenY = Canvas->ClipY - 100.f;
    FLinearColor TextColor = FLinearColor::White;

    FCanvasTextItem TextItem(FVector2D(ScreenX, ScreenY), FText::FromString(AmmoText), GEngine->GetSmallFont(), TextColor);
    TextItem.EnableShadow(FLinearColor::Black);
    Canvas->DrawItem(TextItem);
}