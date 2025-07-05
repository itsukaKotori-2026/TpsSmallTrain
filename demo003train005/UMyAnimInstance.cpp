// Fill out your copyright notice in the Description page of Project Settings.


#include "UMyAnimInstance.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Character.h"
UUMyAnimInstance::UUMyAnimInstance()
{
	UE_LOG(LogTemp, Warning, TEXT("UMyAnimInstance constructor called."));
	Speed = 0.0f;
	bIsInAir = false;
	bIsJumping = false;
}
void UUMyAnimInstance::NativeUpdateAnimation(float DeltaTimeX)
{
	Super::NativeUpdateAnimation(DeltaTimeX);
	UE_LOG(LogTemp, Warning, TEXT("UMyAnimInstance Initialized."));
	// 获取角色
	ACharacter* Character = Cast<ACharacter>(TryGetPawnOwner());
	if (Character)
	{
		// 计算角色的速度
		Speed = Character->GetVelocity().Size();

		// 判断角色是否在空中 (是否处于Falling状态)
		UCharacterMovementComponent* CharMovement = Character->GetCharacterMovement();
		if (CharMovement)
		{
			// 判断角色是否在空中 (掉落、跳跃等)
			bIsInAir = CharMovement->IsFalling();

			// 使用MovementMode来判断是否在跳跃或自由落体状态
			if (CharMovement->MovementMode == MOVE_Falling)
			{
				// 如果角色处于Falling模式，即为跳跃/自由落体
				bIsJumping = true;
			}
			else
			{
				// 角色不在Falling状态，跳跃状态结束
				bIsJumping = false;
			}
		}
	}
}