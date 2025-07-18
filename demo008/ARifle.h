// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AWeapon.h"
#include "ARifle.generated.h"

/**
 * 
 */
UCLASS()
class DEMO008_API AARifle : public AAWeapon
{
	GENERATED_BODY()
public:
    // 构造函数
    AARifle();

    // 重写 Fire 方法（可根据需要自定义）
    virtual void Fire() override;
};
