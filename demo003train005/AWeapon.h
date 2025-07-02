// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AWeapon.generated.h"

UCLASS()
class DEMO003TRAIN005_API AAWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAWeapon();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	int32 MaxAmmo = 30;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Weapon")
	int32 CurrentAmmo;

	// 武器的骨骼网格组件（只保留这一个声明）
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Weapon")
	USkeletalMeshComponent* Mesh;

	void Fire();         // 射击逻辑
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

};
