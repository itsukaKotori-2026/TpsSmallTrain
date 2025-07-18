// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameMode.h"
#include "TargetActor.h" //
#include "Engine/World.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "MyHUD.h"

AMyGameMode::AMyGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
    // ÉèÖÃÄ¬ÈÏ HUD Àà
    HUDClass = AMyHUD::StaticClass();

}
void AMyGameMode::BeginPlay()
{
    Super::BeginPlay();

    UWorld* World = GetWorld();
    if (World)
    {
        FVector SpawnLocation(0.f, 0.f, 300.f); // 
        FRotator SpawnRotation = FRotator(0.f, 0.f, 0.f);

        FActorSpawnParameters SpawnParams;

        AActor* SpawnedTarget = World->SpawnActor<ATargetActor>(
            ATargetActor::StaticClass(),
            SpawnLocation,
            SpawnRotation,
            SpawnParams
        );
    }
}