// Fill out your copyright notice in the Description page of Project Settings.


#include "AWeapon.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"

// Sets default values
AAWeapon::AAWeapon()
{
    // 设置这个 Actor 每帧调用 Tick()，如果不需要可以关闭提升性能
    PrimaryActorTick.bCanEverTick = true;
    // 创建武器网格组件
	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	RootComponent = Mesh;
    // 设置网格为 KA47 武器
    static ConstructorHelpers::FObjectFinder<USkeletalMesh> KA47Mesh(TEXT("/Game/FPS_Weapon_Bundle/Weapons/Meshes/Ka47/SK_KA47.SK_KA47"));

    if (KA47Mesh.Succeeded())
    {
        UE_LOG(LogTemp, Warning, TEXT("Successfully obtained weapon blueprint"));
        Mesh->SetSkeletalMesh(KA47Mesh.Object);
        Mesh->SetAnimationMode(EAnimationMode::AnimationBlueprint);
    }
    // 默认弹药
	CurrentAmmo = MaxAmmo;

}
void AAWeapon::Fire()
{
    // 获取射击的起始位置，通常是枪口的 Socket
    FVector Start = Mesh->GetSocketLocation("Muzzle");
    FVector End = Start + Mesh->GetForwardVector() * 10000; // 射程可以调整，10000是一个大的值
    // 创建命中结果
    FHitResult Hit;
    FCollisionQueryParams Params;
    Params.AddIgnoredActor(this);// 忽略当前武器的碰撞
    // 执行射线检测
    if (GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, Params))
    {
        // 如果命中目标，画一条红色的调试线
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

