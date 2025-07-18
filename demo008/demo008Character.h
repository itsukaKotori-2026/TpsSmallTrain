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

	/**  ������� CameraBoom */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** ������� FollowCamera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;
public:
	Ademo008Character();

	/** �������ת BaseTurnRate */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseTurnRate;

	/** ������¿� BaseLookUpRate */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseLookUpRate;

protected:

	/** Resets HMD orientation in VR. */
	void OnResetVR();

	/** ǰ���ƶ� MoveForward  */
	void MoveForward(float Value);

	/** �����ƶ� MoveForward  */
	void MoveRight(float Value);

	/** �������ת TurnAtRate */
	void TurnAtRate(float Rate);

	/** �������ת LookUpAtRate */
	void LookUpAtRate(float Rate);

	/** ��ʼ����ʱ���� TouchStarted */
	void TouchStarted(ETouchIndex::Type FingerIndex, FVector Location);

	/** ����ֹͣʱ���� TouchStopped */
	void TouchStopped(ETouchIndex::Type FingerIndex, FVector Location);

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// End of APawn interface

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	void StartAiming();//��׼��
	void StopAiming();//����׼
	void StartFire(); //�������
	void StopFire(); //ֹͣ���
	void OnFire(); // ʵ������߼�
	void OnReload(); //����

	UPROPERTY()
	TArray<AAWeapon*> Inventory; //��ɫ���е�����

	UPROPERTY()
	int32 CurrentWeaponIndex = -1; //��ɫ��ǰװ��������

	void SetupWeaponInventory();
	void EquipWeapon(int32 Index); //װ��ĳ������
	void NextWeapon(); //�л�����һ������
	void PrevWeapon(); //�л�����һ������
	void HandleMouseWheel(float Value);
public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	FORCEINLINE float GetCrosshairSpread() const { return CrosshairSpread; }


	/** Ĭ��������ͼ�� */
	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	TSubclassOf<AAWeapon> DefaultWeaponClass;  // ����ָ��Ĭ��������ͼ

	class AAWeapon* GetEquippedWeapon() const { return EquippedWeapon; }

	void EquipWeaponSlot1() { EquipWeapon(0); }
	void EquipWeaponSlot2() { EquipWeapon(1); }

	UPROPERTY(EditDefaultsOnly, Category = "Weapons")
TSubclassOf<class AAWeapon> DefaultWeaponClass1; // ������

UPROPERTY(EditDefaultsOnly, Category = "Weapons")
TSubclassOf<class AAWeapon> DefaultWeaponClass2; // ������

private:
	bool bIsAiming; // �Ƿ�������׼

	// ����׼����ɢ
	float CrosshairSpread = 0.0f; //��ǰ׼�ǵ���ɢ�̶�
	float MaxCrosshairSpread = 30.0f; //׼����ɢ�����ֵ
	float SpreadIncreaseRate = 30.0f;   // ÿ�������ɢ����
	float SpreadDecreaseRate = 20.0f;   // ֹͣ����ָ�����
	bool bIsFiring = false;
	// ���ƽ�ɫ���������
	FTimerHandle FireTimerHandle;
	// ����������λ���룩���� 0.1f Ϊÿ��10��
	float FireRate = 0.1f;

	/** ������� */
	UPROPERTY(VisibleAnywhere, Category = "Weapon")
	AAWeapon* EquippedWeapon;
};

