// Fill out your copyright notice in the Description page of Project Settings.


#include "AWeapon.h"
#include "TargetActor.h"
#include "demo008Character.h"
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

    // 默认弹药
    CurrentAmmo = MaxAmmo;
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
void AAWeapon::Fire()
{

    ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwner());
    if (!OwnerCharacter) return;

    // 获取摄像机组件
    UCameraComponent* CameraComp = OwnerCharacter->FindComponentByClass<UCameraComponent>();
    if (!CameraComp) return;
    // 获取扩散值
    float SpreadAngle = 0.0f;
    Ademo008Character* Shooter = Cast<Ademo008Character>(OwnerCharacter);
    if (Shooter)
    {
        SpreadAngle = Shooter->GetCrosshairSpread();
    }
    FVector Start = CameraComp->GetComponentLocation();
    FVector Direction = CameraComp->GetForwardVector();

    // 应用扩散：生成一个在锥体内的方向
    FVector ShootDir = FMath::VRandCone(Direction, FMath::DegreesToRadians(SpreadAngle));
    FVector End = Start + ShootDir * 10000.0f;

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
int32 AAWeapon::GetCurrentAmmo() const
{
    return CurrentAmmo;
}
int32 AAWeapon::GetTotalAmmo() const
{
    return TotalAmmo;
}

void AAWeapon::StartReload()
{
    if (bIsReloading || CurrentAmmo == MaxAmmo || TotalAmmo <= 0) return;

    bIsReloading = true;

    UE_LOG(LogTemp, Warning, TEXT("StartReload(): 开始换弹中..."));

    GetWorld()->GetTimerManager().SetTimer(
        ReloadTimerHandle,
        this,
        &AAWeapon::FinishReload,
        ReloadDuration,
        false
    );
}

void AAWeapon::FinishReload()
{
    int32 AmmoNeeded = MaxAmmo - CurrentAmmo;
    int32 AmmoToReload = FMath::Min(AmmoNeeded, TotalAmmo);

    CurrentAmmo += AmmoToReload;
    TotalAmmo -= AmmoToReload;

    bIsReloading = false;

    UE_LOG(LogTemp, Warning, TEXT("FinishReload(): 换弹完成，当前子弹：%d / %d"), CurrentAmmo, TotalAmmo);
}
