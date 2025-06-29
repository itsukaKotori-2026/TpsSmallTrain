// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameMode.h"
#include "TargetActor.h" // ��İ�����
#include "Engine/World.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"
void AMyGameMode::BeginPlay()
{
    Super::BeginPlay();

    UWorld* World = GetWorld();
    if (World)
    {
        FVector SpawnLocation(0.f, 0.f, 300.f); // ����λ��
        FRotator SpawnRotation = FRotator(0.f, 0.f, 0.f);
        UE_LOG(LogTemp, Warning, TEXT("Spawning at location: %s"), *SpawnLocation.ToString());
        FActorSpawnParameters SpawnParams;

        AActor* SpawnedTarget = World->SpawnActor<ATargetActor>(
            ATargetActor::StaticClass(),
            SpawnLocation,
            SpawnRotation,
            SpawnParams
        );

        if (SpawnedTarget)
        {
            UE_LOG(LogTemp, Warning, TEXT("Successfully generated target!"));
        }
    }
}