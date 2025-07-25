// Fill out your copyright notice in the Description page of Project Settings.


#include "AZombie.h"
#include "ZombieAIController.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Actor.h"
#include "DrawDebugHelpers.h"
// Sets default values
AAZombie::AAZombie()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	// Set the default health value
	Health = 100.f; // 设置默认生命值
	DetectionRange = 500.f;  // 设置追踪玩家的检测距离
	//放在构造函数中
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> ZombieMesh(TEXT("SkeletalMesh'/Game/Mannequin/Character/Mesh/SK_Mannequin.SK_Mannequin'"));
	if (ZombieMesh.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(ZombieMesh.Object);  // 设置模型
		GetMesh()->SetRelativeLocation(FVector(0.f, 0.f, 30.f));//僵尸角色位置，离地面高30cm
		GetMesh()->SetRelativeRotation(FRotator(0.f, 90.f, 0.f)); //僵尸角色朝向
	}
}

// Called when the game starts or when spawned
void AAZombie::BeginPlay()
{
	Super::BeginPlay();
	// Get the AI controller of the zombie

	AIController = Cast<AZombieAIController>(GetController());
	if (!AIController)
	{
		UE_LOG(LogTemp, Warning, TEXT("Zombie has no AIController!"));
	}
}

// Called every frame
void AAZombie::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	AActor* PlayerActor = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	if (AIController && PlayerActor)
	{
		// Calculate the distance to the player
		float DistanceToPlayer = FVector::Dist(PlayerActor->GetActorLocation(), GetActorLocation());

		// If within detection range, start chasing the player
		if (DistanceToPlayer <= DetectionRange)
		{
			// Move towards the player
			AIController->MoveToActor(PlayerActor);
		}

		// If too close, attack the player
		if (DistanceToPlayer <= 100.f) // Set the attack range
		{
			AttackPlayer();
		}
	}
}

// Called to bind functionality to input
void AAZombie::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}
void AAZombie::AttackPlayer()
{
	// Implement logic to attack the player, e.g., reduce player health
	// For now, just log the attack
	UE_LOG(LogTemp, Warning, TEXT("Zombie Attacking Player"));

	// Implement damage to player (can be done by calling a method on player character)
	AActor* PlayerActor = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	if (PlayerActor)
	{
		// You can call a method on the player character to reduce health
		// PlayerActor->TakeDamage(10.f);
	}
}

