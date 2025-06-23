//��Ȩ����Epic Games���ɷ����޹�˾��������Ȩ����

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "firstTpsDemo002Character.generated.h"

UCLASS(config=Game)
class AfirstTpsDemo002Character : public ACharacter
{
	GENERATED_BODY()

	/** ������˽������λ�ڽ�ɫ���� */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** ��������ͷ */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;
public:
	AfirstTpsDemo002Character();

	/** ����ת�����ʣ���λΪ��/�롣�������ſ��ܻ�Ӱ������ת������ */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseTurnRate;

	/** ������/�²������ʣ���λΪ��/�롣�������ſ��ܻ�Ӱ���������ʡ� */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseLookUpRate;

protected:

	/** ����VR�е�HMD���� */
	void OnResetVR();

	/** ��Ҫ��ǰ/������� */
	void MoveForward(float Value);

	/** ��Ҫ�������� */
	void MoveRight(float Value);

	/** 
	 * ͨ����������Ը�������ת���� 
	 * @param Rate	����һ����׼���ʣ���1.0��ʾ����ת���ʵ�100%
	 */
	void TurnAtRate(float Rate);

	/**
	 * ͨ��������ã��Ը�������������/���²���
	 * @param Rate	����һ����׼���ʣ���1.0��ʾ����ת���ʵ�100%
	 */
	void LookUpAtRate(float Rate);

	/** �������뿪ʼʱ�Ĵ������ */
	void TouchStarted(ETouchIndex::Type FingerIndex, FVector Location);

	/** ��������ֹͣʱ�Ĵ������ */
	void TouchStopped(ETouchIndex::Type FingerIndex, FVector Location);

protected:
	// APawn�ӿ�
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// APawn�������
	void OnFire();
public:
	/** ����CameraRoom�Ӷ��� **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** ����FollowCamera�Ӷ��� **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
};

