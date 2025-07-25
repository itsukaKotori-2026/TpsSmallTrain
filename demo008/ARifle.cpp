// Fill out your copyright notice in the Description page of Project Settings.


#include "ARifle.h"

#include "UObject/ConstructorHelpers.h"
#include "Engine/SkeletalMesh.h"
#include "Components/SkeletalMeshComponent.h"
// 构造函数
AARifle::AARifle()
{
    // 设置默认的 Skeletal Mesh 资源
    static ConstructorHelpers::FObjectFinder<USkeletalMesh> AR4Mesh(TEXT("/Game/FPS_Weapon_Bundle/Weapons/Meshes/AR4/SK_AR4.SK_AR4"));

    if (AR4Mesh.Succeeded())
    {
        UE_LOG(LogTemp, Warning, TEXT("Successfully loaded AR4 Mesh"));
        Mesh->SetSkeletalMesh(AR4Mesh.Object);
        // 设置动画模式为 Blueprint（如果有动画蓝图的话）
        Mesh->SetAnimationMode(EAnimationMode::AnimationBlueprint);
    }
}
