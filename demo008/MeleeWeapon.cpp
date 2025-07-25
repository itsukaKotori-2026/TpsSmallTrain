// Fill out your copyright notice in the Description page of Project Settings.


#include "MeleeWeapon.h"
#include "UObject/ConstructorHelpers.h"
#include "Engine/SkeletalMesh.h"
#include "Components/SkeletalMeshComponent.h"
// Sets default values
AMeleeWeapon::AMeleeWeapon()
{
    static ConstructorHelpers::FObjectFinder<USkeletalMesh> G67Mesh(TEXT("/Game/FPS_Weapon_Bundle/Weapons/Meshes/M9_Knife/SK_M9_Knife.SK_M9_Knife"));
    if (G67Mesh.Succeeded())
    {
        Mesh->SetSkeletalMesh(G67Mesh.Object);
        Mesh->SetAnimationMode(EAnimationMode::AnimationBlueprint);
    }
}


