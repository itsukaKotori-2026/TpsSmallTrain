// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once
#include "AWeapon.h"
#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "demo007Character.generated.h"

UCLASS(config=Game)
class Ademo007Character : public ACharacter
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;
public:
	Ademo007Character();

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseLookUpRate;

protected:

	/** Resets HMD orientation in VR. */
	void OnResetVR();

	/** Called for forwards/backward input */
	void MoveForward(float Value);

	/** Called for side to side input */
	void MoveRight(float Value);

	/** 
	 * Called via input to turn at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);

	/** Handler for when a touch input begins. */
	void TouchStarted(ETouchIndex::Type FingerIndex, FVector Location);

	/** Handler for when a touch input stops. */
	void TouchStopped(ETouchIndex::Type FingerIndex, FVector Location);

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// End of APawn interface
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	void StartAiming();
	void StopAiming();
	void StartFire();
	void StopFire();
	void OnFire();
	void OnReload();
public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
	FORCEINLINE float GetCrosshairSpread() const { return CrosshairSpread; }
	/** 武器相关 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
	AAWeapon* EquippedWeapon;

	/** 默认武器蓝图类 */
	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	TSubclassOf<AAWeapon> DefaultWeaponClass;  // 用于指定默认武器蓝图

	class AAWeapon* GetEquippedWeapon() const { return EquippedWeapon; }
private:
	bool bIsAiming; // 是否正在瞄准
	// 控制自动开火
	FTimerHandle FireTimerHandle;
	// 控制准星扩散
	float CrosshairSpread = 0.0f;
	float MaxCrosshairSpread = 30.0f;
	float SpreadIncreaseRate = 30.0f;   // 每秒增加多少
	float SpreadDecreaseRate = 20.0f;   // 每秒减少多少
	bool bIsFiring = false;

	// 射击间隔（单位：秒）例如 0.1f 为每秒10发
	float FireRate = 0.1f;
};

