// Fill out your copyright notice in the Description page of Project Settings.


#include "ZombieAIController.h"
#include "AZombie.h"
void AZombieAIController::BeginPlay()
{
	Super::BeginPlay();
}

void AZombieAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	AAZombie* Zombie = Cast<AAZombie>(GetPawn());
	if (Zombie)
	{
		// You can implement behavior here (patrol, chase, etc.)
	}
}