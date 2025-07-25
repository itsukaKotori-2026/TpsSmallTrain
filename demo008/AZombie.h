// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AZombie.generated.h"

UCLASS()
class DEMO008_API AAZombie : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AAZombie();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Attack logic
	void AttackPlayer();
	// Health system
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Health = 100.f;
	// Player detection and movement
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DetectionRange = 500.f;
private:
	// AI Controller for the zombie
	UPROPERTY()
	AZombieAIController* AIController;
};
