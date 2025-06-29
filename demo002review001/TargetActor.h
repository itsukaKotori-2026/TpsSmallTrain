// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TargetActor.generated.h"

UCLASS()
class DEMO002REVIEW001_API ATargetActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATargetActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	// 命中时调用
	//myadd
	void OnHitByBullet();
//myadd
private:
	//UPROPERTY：告诉引擎这变量参与反射系统、序列化等。
	//VisibleAnywhere：表示在 编辑器中（蓝图或详情面板）可见，但不可编辑。
	UPROPERTY(VisibleAnywhere)

	//这是一个指向 静态网格体组件 的指针。 它代表这个靶子的外观（3D 模型）。
	UStaticMeshComponent* TargetMesh;
	//这是一个布尔值， true 表示靶子正在倒下 false 表示靶子静止
	bool bIsFalling;
	//这是一个三维向量，表示 靶子倒下的方向。 比如：FVector(1, 0, 0) 就表示沿 X 轴正方向倒下。
	FVector FallDirection;
	//一个浮点数，表示： 靶子倒下时的速度（单位通常是单位 / 秒）。
	float FallSpeed;
};

