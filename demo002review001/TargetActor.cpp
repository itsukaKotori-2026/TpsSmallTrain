// Fill out your copyright notice in the Description page of Project Settings.


#include "TargetActor.h"
//myadd
#include "UObject/ConstructorHelpers.h"

// Sets default values
ATargetActor::ATargetActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	//myadd
	TargetMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TargetMesh"));
	RootComponent = TargetMesh;
	// 自动加载引擎内置 Cylinder 模型，如果加载成功，将其赋给 TargetMesh。
	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset(TEXT("/Engine/BasicShapes/Cylinder.Cylinder"));
	if (MeshAsset.Succeeded())
	{
		UE_LOG(LogTemp, Warning, TEXT("Mesh loaded successfully."));
		TargetMesh->SetStaticMesh(MeshAsset.Object);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Mesh load FAILED."));
	}
	// 自动加载引擎内置材质，如果加载成功，将其赋给 TargetMesh。
	static ConstructorHelpers::FObjectFinder<UMaterial> MaterialAsset(TEXT("/Engine/BasicShapes/BasicShapeMaterial"));
	if (MaterialAsset.Succeeded())
	{
		TargetMesh->SetMaterial(0, MaterialAsset.Object);
	}
	// 设置缩放，像个靶子的圆盘
	TargetMesh->SetWorldScale3D(FVector(1.0f, 1.0f, 0.05f));
	// 新增旋转，让圆柱“立起来”
	TargetMesh->SetRelativeRotation(FRotator(0.f, 0.f, 90.f));
	// 启用碰撞
	//开启查询和物理碰撞（既能检测射线 / 重叠，也能物理交互）
	//设置对象类型为 WorldDynamic（动态物体，比如可以移动的物体）
	//对所有碰撞通道阻挡（子弹、角色、射线都会被挡住）
	TargetMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	TargetMesh->SetCollisionObjectType(ECC_WorldDynamic);
	TargetMesh->SetCollisionResponseToAllChannels(ECR_Block);

	// 确保物理没有干扰 SetActorRotation
	TargetMesh->SetSimulatePhysics(false);
	//  初始化：
	//bIsFalling = false：开始时靶子不倒下。
	//FallSpeed = 100.0f：倒下的旋转速度，单位角度 / 秒。
	bIsFalling = false;
	FallSpeed = 100.0f;
}

// Called when the game starts or when spawned
void ATargetActor::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(LogTemp, Warning, TEXT("Target mesh initial forward vector: %s"), *TargetMesh->GetForwardVector().ToString());
	UE_LOG(LogTemp, Warning, TEXT("Target mesh initial up vector: %s"), *TargetMesh->GetUpVector().ToString());
	
}

// Called every frame
void ATargetActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//myadd
	if (bIsFalling) {//检查 bIsFalling 是否为 true（是否被击中）。

		// 获取当前旋转角度
		FRotator NewRotation = TargetMesh->GetRelativeRotation();
		NewRotation.Roll += FallSpeed * DeltaTime; // 每帧增加一定的旋转值


		// 当 Pitch 达到或超过 90 度（靶子完全倒下）时，停止旋转（bIsFalling = false）。
		if (NewRotation.Roll >= 0.0f)
		{
			NewRotation.Roll = 0.0f; // 防止超过 90°
			bIsFalling = false;  // 停止旋转

		}
		// 更新旋转
		TargetMesh->SetRelativeRotation(NewRotation);
		UE_LOG(LogTemp, Warning, TEXT("Target is falling... Current roll: %.2f"), NewRotation.Roll);

	}
}
//myadd
//当靶子被子弹击中时调用，启动倒下动画。由外部（如角色的 OnFire() 射线检测）调用。
void ATargetActor::OnHitByBullet()
{
	if (!bIsFalling) //仅当靶子未倒下时（!bIsFalling）才会触发。
	{
		bIsFalling = true; // 触发倒下
		UE_LOG(LogTemp, Warning, TEXT("Target hit by bullet - starting fall animation..."));
	}
}

