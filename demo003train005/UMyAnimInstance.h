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
	UUMyAnimInstance(); // 构造函数声明

	virtual void NativeUpdateAnimation(float DeltaTimeX) override;  // 每帧更新动画
protected:
	// 动画参数（如速度、是否在空中等）
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	float Speed;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	bool bIsInAir;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	bool bIsJumping;
};
