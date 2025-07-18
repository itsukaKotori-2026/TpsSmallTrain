// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AWeapon.generated.h"

UCLASS()
class DEMO008_API AAWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAWeapon();
	// ����߼�
	virtual void Fire();

	// ��ҩ�߼�
	void DecreaseAmmo(int32 Amount) { CurrentAmmo -= Amount; }
	int32 GetCurrentAmmo() const;
	int32 GetTotalAmmo() const;
	// === �������ķ��� ===
	void StartReload();
	void FinishReload();
	bool IsReloading() const { return bIsReloading; }

	// �����Ĺ������������ֻ������һ��������
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Weapon")
	USkeletalMeshComponent* Mesh;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
private:
	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	int32 MaxAmmo = 30;

	UPROPERTY(VisibleAnywhere, Category = "Weapon")
	int32 CurrentAmmo = 30;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	int32 TotalAmmo = 540;
	// ========== ����ϵͳ ==========
	bool bIsReloading = false;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	float ReloadDuration = 2.3f;

	FTimerHandle ReloadTimerHandle;
};
