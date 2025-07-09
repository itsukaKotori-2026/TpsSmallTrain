// Fill out your copyright notice in the Description page of Project Settings.


#include "AWeapon.h"
#include "TargetActor.h"
#include "demo007Character.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/Character.h"
// Sets default values
AAWeapon::AAWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
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

    // ��ȡ��������
    UCameraComponent* CameraComp = OwnerCharacter->FindComponentByClass<UCameraComponent>();
    if (!CameraComp) return;
    // ��ȡ��ɢֵ
    float SpreadAngle = 0.0f;
    Ademo007Character* Shooter = Cast<Ademo007Character>(OwnerCharacter);
    if (Shooter)
    {
        SpreadAngle = Shooter->GetCrosshairSpread();
    }
    FVector Start = CameraComp->GetComponentLocation();
    FVector Direction = CameraComp->GetForwardVector();

    // Ӧ����ɢ������һ����׶���ڵķ���
    FVector ShootDir = FMath::VRandCone(Direction, FMath::DegreesToRadians(SpreadAngle));
    FVector End = Start + ShootDir * 10000.0f;

    FHitResult Hit;
    FCollisionQueryParams Params;
    Params.AddIgnoredActor(this);
    Params.AddIgnoredActor(OwnerCharacter); // �����Լ�

    if (GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, Params))
    {
        DrawDebugLine(GetWorld(), Start, Hit.Location, FColor::Red, false, 1.f);
        UE_LOG(LogTemp, Warning, TEXT("Hit something at: %s"), *Hit.Location.ToString());

        // 
        ATargetActor* Target = Cast<ATargetActor>(Hit.GetActor());
        if (Target)
        {
            Target->OnHitByBullet(); //֪ͨ���ӱ�����
        }
    }
    else
    {
        DrawDebugLine(GetWorld(), Start, End, FColor::Blue, false, 1.f); // û����Ҳ����
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

    UE_LOG(LogTemp, Warning, TEXT("StartReload(): ��ʼ������..."));

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

    UE_LOG(LogTemp, Warning, TEXT("FinishReload(): ������ɣ���ǰ�ӵ���%d / %d"), CurrentAmmo, TotalAmmo);
}