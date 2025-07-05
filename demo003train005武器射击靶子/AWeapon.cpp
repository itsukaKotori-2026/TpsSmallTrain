// Fill out your copyright notice in the Description page of Project Settings.


#include "AWeapon.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"

// Sets default values
AAWeapon::AAWeapon()
{
    // ������� Actor ÿ֡���� Tick()���������Ҫ���Թر���������
    PrimaryActorTick.bCanEverTick = true;
    // ���������������
	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	RootComponent = Mesh;
    // ��������Ϊ KA47 ����
    static ConstructorHelpers::FObjectFinder<USkeletalMesh> KA47Mesh(TEXT("/Game/FPS_Weapon_Bundle/Weapons/Meshes/Ka47/SK_KA47.SK_KA47"));

    if (KA47Mesh.Succeeded())
    {
        UE_LOG(LogTemp, Warning, TEXT("Successfully obtained weapon blueprint"));
        Mesh->SetSkeletalMesh(KA47Mesh.Object);
        Mesh->SetAnimationMode(EAnimationMode::AnimationBlueprint);
    }
    // Ĭ�ϵ�ҩ
	CurrentAmmo = MaxAmmo;

}
void AAWeapon::Fire()
{
    // ��ȡ�������ʼλ�ã�ͨ����ǹ�ڵ� Socket
    FVector Start = Mesh->GetSocketLocation("Muzzle");
    FVector End = Start + Mesh->GetForwardVector() * 10000; // ��̿��Ե�����10000��һ�����ֵ
    // �������н��
    FHitResult Hit;
    FCollisionQueryParams Params;
    Params.AddIgnoredActor(this);// ���Ե�ǰ��������ײ
    // ִ�����߼��
    if (GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, Params))
    {
        // �������Ŀ�꣬��һ����ɫ�ĵ�����
        DrawDebugLine(GetWorld(), Start, Hit.Location, FColor::Red, false, 1.f);
    }
}


// Called when the game starts or when spawned
void AAWeapon::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AAWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

