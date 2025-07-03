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
    // ��ȡ�������ʼλ�ã�ͨ����ǹ�ڵ� Socket
    //FVector Start = Mesh->GetSocketLocation("Muzzle");
    //FVector End = Start + Mesh->GetForwardVector() * 10000; // ��̿��Ե�����10000��һ�����ֵ
    //// �������н��
    //FHitResult Hit;
    //FCollisionQueryParams Params;
    //Params.AddIgnoredActor(this);// ���Ե�ǰ��������ײ
    //// ִ�����߼��
    //if (GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, Params))
    //{
    //    // �������Ŀ�꣬��һ����ɫ�ĵ�����
    //    DrawDebugLine(GetWorld(), Start, Hit.Location, FColor::Red, false, 1.f);
    //}
    ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwner());
    if (!OwnerCharacter) return;

    // ��ȡ��������
    UCameraComponent* CameraComp = OwnerCharacter->FindComponentByClass<UCameraComponent>();
    if (!CameraComp) return;

    FVector Start = CameraComp->GetComponentLocation();
    FVector Direction = CameraComp->GetForwardVector();
    FVector End = Start + Direction * 10000;

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