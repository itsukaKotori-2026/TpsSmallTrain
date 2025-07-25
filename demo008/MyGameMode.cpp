// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameMode.h"
#include "TargetActor.h" //
#include "Engine/World.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "AZombie.h"
#include "MyHUD.h"

AMyGameMode::AMyGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
    // 设置默认 HUD 类
    HUDClass = AMyHUD::StaticClass();

}
void AMyGameMode::BeginPlay()
{
    Super::BeginPlay();

    UWorld* World = GetWorld();
    if (World)
    {
        FVector TargetSpawnLocation(0.f, 0.f, 300.f); // 
        FRotator  TargetSpawnRotation = FRotator(0.f, 0.f, 0.f);

        FActorSpawnParameters TargetSpawnParams;

        AActor* SpawnedTarget = World->SpawnActor<ATargetActor>(
            ATargetActor::StaticClass(),
            TargetSpawnLocation,
            TargetSpawnRotation,
            TargetSpawnParams
        );

        FVector ZombieSpawnLocation(0.f, 0.f, 100.f); // 定义僵尸的生成位置
        FRotator ZombieSpawnRotation = FRotator::ZeroRotator; //设置生成时的朝向
        //生成时有物体挡住，就尝试稍微移动生成位置,避免因为碰撞问题导致生成失败
        FActorSpawnParameters ZombieSpawnParams;
        ZombieSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
        //GetWorld()：获取当前的游戏世界上下文,生成后，返回一个指向新 AAZombie 实例的指针
        AAZombie* SpawnedZombie = GetWorld()->SpawnActor<AAZombie>(AAZombie::StaticClass(), ZombieSpawnLocation, ZombieSpawnRotation, ZombieSpawnParams);

        if (SpawnedZombie)
        {
            UE_LOG(LogTemp, Warning, TEXT("Zombie spawned successfully"));
        }
    }
}