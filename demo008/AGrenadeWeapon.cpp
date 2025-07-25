// Fill out your copyright notice in the Description page of Project Settings.


#include "AGrenadeWeapon.h"
#include "UObject/ConstructorHelpers.h"
#include "Engine/SkeletalMesh.h"
#include "Components/SkeletalMeshComponent.h"
// Sets default values
AAGrenadeWeapon::AAGrenadeWeapon()
{
    static ConstructorHelpers::FObjectFinder<USkeletalMesh> G67Mesh(TEXT("/Game/FPS_Weapon_Bundle/Weapons/Meshes/G67_Grenade/SK_G67_X.SK_G67_X"));
    if (G67Mesh.Succeeded())
    {   
        // 使用继承自AWeapon的Mesh组件
        Mesh->SetSkeletalMesh(G67Mesh.Object);
        Mesh->SetAnimationMode(EAnimationMode::AnimationBlueprint);
    }
}

