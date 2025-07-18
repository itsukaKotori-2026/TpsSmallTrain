// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AWeapon.h"
#include "AKA47Weapon.h"
#include "ARifle.h"
#include "demo008Character.generated.h"
UCLASS(config=Game)
class Ademo008Character : public ACharacter
{
	GENERATED_BODY()

	/**  相机吊杆 CameraBoom */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** 跟随相机 FollowCamera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;
public:
	Ademo008Character();

	/** 鼠标左右转 BaseTurnRate */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseTurnRate;

	/** 鼠标上下看 BaseLookUpRate */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseLookUpRate;

protected:

	/** Resets HMD orientation in VR. */
	void OnResetVR();

	/** 前后移动 MoveForward  */
	void MoveForward(float Value);

	/** 左右移动 MoveForward  */
	void MoveRight(float Value);

	/** 鼠标左右转 TurnAtRate */
	void TurnAtRate(float Rate);

	/** 鼠标上下转 LookUpAtRate */
	void LookUpAtRate(float Rate);

	/** 开始触摸时调用 TouchStarted */
	void TouchStarted(ETouchIndex::Type FingerIndex, FVector Location);

	/** 触摸停止时调用 TouchStopped */
	void TouchStopped(ETouchIndex::Type FingerIndex, FVector Location);

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// End of APawn interface

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	void StartAiming();//瞄准中
	void StopAiming();//不瞄准
	void StartFire(); //启动射击
	void StopFire(); //停止射击
	void OnFire(); // 实际射击逻辑
	void OnReload(); //换弹

	UPROPERTY()
	TArray<AAWeapon*> Inventory; //角色所有的武器

	UPROPERTY()
	int32 CurrentWeaponIndex = -1; //角色当前装备的武器

	void SetupWeaponInventory();
	void EquipWeapon(int32 Index); //装备某个武器
	void NextWeapon(); //切换到下一个武器
	void PrevWeapon(); //切换到上一个武器
	void HandleMouseWheel(float Value);
public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	FORCEINLINE float GetCrosshairSpread() const { return CrosshairSpread; }


	/** 默认武器蓝图类 */
	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	TSubclassOf<AAWeapon> DefaultWeaponClass;  // 用于指定默认武器蓝图

	class AAWeapon* GetEquippedWeapon() const { return EquippedWeapon; }

	void EquipWeaponSlot1() { EquipWeapon(0); }
	void EquipWeaponSlot2() { EquipWeapon(1); }

	UPROPERTY(EditDefaultsOnly, Category = "Weapons")
TSubclassOf<class AAWeapon> DefaultWeaponClass1; // 主武器

UPROPERTY(EditDefaultsOnly, Category = "Weapons")
TSubclassOf<class AAWeapon> DefaultWeaponClass2; // 副武器

private:
	bool bIsAiming; // 是否正在瞄准

	// 控制准星扩散
	float CrosshairSpread = 0.0f; //当前准星的扩散程度
	float MaxCrosshairSpread = 30.0f; //准星扩散的最大值
	float SpreadIncreaseRate = 30.0f;   // 每次射击扩散速率
	float SpreadDecreaseRate = 20.0f;   // 停止射击恢复速率
	bool bIsFiring = false;
	// 控制角色的射击速率
	FTimerHandle FireTimerHandle;
	// 射击间隔（单位：秒）例如 0.1f 为每秒10发
	float FireRate = 0.1f;

	/** 武器相关 */
	UPROPERTY(VisibleAnywhere, Category = "Weapon")
	AAWeapon* EquippedWeapon;
};

