//版权所有Epic Games，股份有限公司保留所有权利。

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "firstTpsDemo002Character.generated.h"

UCLASS(config=Game)
class AfirstTpsDemo002Character : public ACharacter
{
	GENERATED_BODY()

	/** 相机吊杆将相机定位在角色后面 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** 跟随摄像头 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;
public:
	AfirstTpsDemo002Character();

	/** 基本转弯速率，单位为度/秒。其他缩放可能会影响最终转弯速率 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseTurnRate;

	/** 基本上/下查找速率，单位为度/秒。其他缩放可能会影响最终速率。 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseLookUpRate;

protected:

	/** 重置VR中的HMD方向。 */
	void OnResetVR();

	/** 需要向前/向后输入 */
	void MoveForward(float Value);

	/** 需要侧向输入 */
	void MoveRight(float Value);

	/** 
	 * 通过输入调用以给定速率转动。 
	 * @param Rate	这是一个标准化率，即1.0表示所需转弯率的100%
	 */
	void TurnAtRate(float Rate);

	/**
	 * 通过输入调用，以给定的速率向上/向下查找
	 * @param Rate	这是一个标准化率，即1.0表示所需转弯率的100%
	 */
	void LookUpAtRate(float Rate);

	/** 触摸输入开始时的处理程序。 */
	void TouchStarted(ETouchIndex::Type FingerIndex, FVector Location);

	/** 触摸输入停止时的处理程序 */
	void TouchStopped(ETouchIndex::Type FingerIndex, FVector Location);

protected:
	// APawn接口
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// APawn界面结束
	void OnFire();
public:
	/** 返回CameraRoom子对象 **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** 返回FollowCamera子对象 **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
};

