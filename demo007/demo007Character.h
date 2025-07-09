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
	/** ������� */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
	AAWeapon* EquippedWeapon;

	/** Ĭ��������ͼ�� */
	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	TSubclassOf<AAWeapon> DefaultWeaponClass;  // ����ָ��Ĭ��������ͼ

	class AAWeapon* GetEquippedWeapon() const { return EquippedWeapon; }
private:
	bool bIsAiming; // �Ƿ�������׼
	// �����Զ�����
	FTimerHandle FireTimerHandle;
	// ����׼����ɢ
	float CrosshairSpread = 0.0f;
	float MaxCrosshairSpread = 30.0f;
	float SpreadIncreaseRate = 30.0f;   // ÿ�����Ӷ���
	float SpreadDecreaseRate = 20.0f;   // ÿ����ٶ���
	bool bIsFiring = false;

	// ����������λ���룩���� 0.1f Ϊÿ��10��
	float FireRate = 0.1f;
};

