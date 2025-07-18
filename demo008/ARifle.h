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
    // ���캯��
    AARifle();

    // ��д Fire �������ɸ�����Ҫ�Զ��壩
    virtual void Fire() override;
};
