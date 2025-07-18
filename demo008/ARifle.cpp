// Fill out your copyright notice in the Description page of Project Settings.


#include "ARifle.h"

#include "UObject/ConstructorHelpers.h"
#include "Engine/SkeletalMesh.h"
#include "Components/SkeletalMeshComponent.h"
// ���캯��
AARifle::AARifle()
{
    // ����Ĭ�ϵ� Skeletal Mesh ��Դ
    static ConstructorHelpers::FObjectFinder<USkeletalMesh> AR4Mesh(TEXT("/Game/FPS_Weapon_Bundle/Weapons/Meshes/AR4/SK_AR4.SK_AR4"));

    if (AR4Mesh.Succeeded())
    {
        UE_LOG(LogTemp, Warning, TEXT("Successfully loaded AR4 Mesh"));
        Mesh->SetSkeletalMesh(AR4Mesh.Object);
        // ���ö���ģʽΪ Blueprint������ж�����ͼ�Ļ���
        Mesh->SetAnimationMode(EAnimationMode::AnimationBlueprint);
    }
}

// ��д Fire ������ִ����������߼�
void AARifle::Fire()
{
    // �������������Ӿ��������߼�
    Super::Fire();
    UE_LOG(LogTemp, Warning, TEXT("Firing ARifle..."));
}