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
	// ��ȡ��ɫ
	ACharacter* Character = Cast<ACharacter>(TryGetPawnOwner());
	if (Character)
	{
		// �����ɫ���ٶ�
		Speed = Character->GetVelocity().Size();

		// �жϽ�ɫ�Ƿ��ڿ��� (�Ƿ���Falling״̬)
		UCharacterMovementComponent* CharMovement = Character->GetCharacterMovement();
		if (CharMovement)
		{
			// �жϽ�ɫ�Ƿ��ڿ��� (���䡢��Ծ��)
			bIsInAir = CharMovement->IsFalling();

			// ʹ��MovementMode���ж��Ƿ�����Ծ����������״̬
			if (CharMovement->MovementMode == MOVE_Falling)
			{
				// �����ɫ����Fallingģʽ����Ϊ��Ծ/��������
				bIsJumping = true;
			}
			else
			{
				// ��ɫ����Falling״̬����Ծ״̬����
				bIsJumping = false;
			}
		}
	}
}