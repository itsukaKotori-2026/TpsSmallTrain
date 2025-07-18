// Fill out your copyright notice in the Description page of Project Settings.


#include "AKA47Weapon.h"

AAKA47Weapon::AAKA47Weapon()
{
    static ConstructorHelpers::FObjectFinder<USkeletalMesh> KA47Mesh(TEXT("/Game/FPS_Weapon_Bundle/Weapons/Meshes/Ka47/SK_KA47.SK_KA47"));
    if (KA47Mesh.Succeeded())
    {
        Mesh->SetSkeletalMesh(KA47Mesh.Object);
        Mesh->SetAnimationMode(EAnimationMode::AnimationBlueprint);
    }
}