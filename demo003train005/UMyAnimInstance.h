// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "UMyAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class DEMO003TRAIN005_API UUMyAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
public:
	UUMyAnimInstance(); // ���캯������

	virtual void NativeUpdateAnimation(float DeltaTimeX) override;  // ÿ֡���¶���
protected:
	// �������������ٶȡ��Ƿ��ڿ��еȣ�
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	float Speed;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	bool bIsInAir;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	bool bIsJumping;
};
