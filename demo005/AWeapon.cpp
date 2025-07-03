// Fill out your copyright notice in the Description page of Project Settings.


#include "AWeapon.h"
#include "TargetActor.h"

#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/Character.h"
// Sets default values
AAWeapon::AAWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
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

// Called when the game starts or when spawned
void AAWeapon::BeginPlay()
{
	Super::BeginPlay();
    if (Mesh)
    {
        FVector Forward = Mesh->GetForwardVector();
        FVector Right = Mesh->GetRightVector();
        FVector Up = Mesh->GetUpVector();

        UE_LOG(LogTemp, Warning, TEXT("Weapon Forward: %s"), *Forward.ToString());
        UE_LOG(LogTemp, Warning, TEXT("Weapon Right:   %s"), *Right.ToString());
        UE_LOG(LogTemp, Warning, TEXT("Weapon Up:      %s"), *Up.ToString());
    }
}

// Called every frame
void AAWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AAWeapon::Fire()
{
    // 获取射击的起始位置，通常是枪口的 Socket
    //FVector Start = Mesh->GetSocketLocation("Muzzle");
    //FVector End = Start + Mesh->GetForwardVector() * 10000; // 射程可以调整，10000是一个大的值
    //// 创建命中结果
    //FHitResult Hit;
    //FCollisionQueryParams Params;
    //Params.AddIgnoredActor(this);// 忽略当前武器的碰撞
    //// 执行射线检测
    //if (GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, Params))
    //{
    //    // 如果命中目标，画一条红色的调试线
    //    DrawDebugLine(GetWorld(), Start, Hit.Location, FColor::Red, false, 1.f);
    //}
    ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwner());
    if (!OwnerCharacter) return;

    // 获取摄像机组件
    UCameraComponent* CameraComp = OwnerCharacter->FindComponentByClass<UCameraComponent>();
    if (!CameraComp) return;

    FVector Start = CameraComp->GetComponentLocation();
    FVector Direction = CameraComp->GetForwardVector();
    FVector End = Start + Direction * 10000;

    FHitResult Hit;
    FCollisionQueryParams Params;
    Params.AddIgnoredActor(this);
    Params.AddIgnoredActor(OwnerCharacter); // 忽略自己

    if (GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, Params))
    {
        DrawDebugLine(GetWorld(), Start, Hit.Location, FColor::Red, false, 1.f);
        UE_LOG(LogTemp, Warning, TEXT("Hit something at: %s"), *Hit.Location.ToString());

        // 
        ATargetActor* Target = Cast<ATargetActor>(Hit.GetActor());
        if (Target)
        {
            Target->OnHitByBullet(); //通知靶子被击中
        }
    }
    else
    {
        DrawDebugLine(GetWorld(), Start, End, FColor::Blue, false, 1.f); // 没命中也画线
        UE_LOG(LogTemp, Warning, TEXT("Missed - tracing to: %s"), *End.ToString());
    }
}