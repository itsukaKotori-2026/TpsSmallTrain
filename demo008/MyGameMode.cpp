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
    // ����Ĭ�� HUD ��
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

        FVector ZombieSpawnLocation(0.f, 0.f, 100.f); // ���彩ʬ������λ��
        FRotator ZombieSpawnRotation = FRotator::ZeroRotator; //��������ʱ�ĳ���
        //����ʱ�����嵲ס���ͳ�����΢�ƶ�����λ��,������Ϊ��ײ���⵼������ʧ��
        FActorSpawnParameters ZombieSpawnParams;
        ZombieSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
        //GetWorld()����ȡ��ǰ����Ϸ����������,���ɺ󣬷���һ��ָ���� AAZombie ʵ����ָ��
        AAZombie* SpawnedZombie = GetWorld()->SpawnActor<AAZombie>(AAZombie::StaticClass(), ZombieSpawnLocation, ZombieSpawnRotation, ZombieSpawnParams);

        if (SpawnedZombie)
        {
            UE_LOG(LogTemp, Warning, TEXT("Zombie spawned successfully"));
        }
    }
}