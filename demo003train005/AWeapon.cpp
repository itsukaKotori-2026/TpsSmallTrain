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

