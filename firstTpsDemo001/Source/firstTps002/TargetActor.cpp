// Fill out your copyright notice in the Description page of Project Settings.


#include "TargetActor.h"
#include "UObject/ConstructorHelpers.h"
// Sets default values
ATargetActor::ATargetActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	TargetMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TargetMesh"));
	RootComponent = TargetMesh;

	// 加载一个引擎自带模型：Cylinder
	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset(TEXT("/Engine/BasicShapes/Cylinder.Cylinder"));
	if (MeshAsset.Succeeded())
	{
		TargetMesh->SetStaticMesh(MeshAsset.Object);
	}

	// 设置缩放：靶子圆盘样子
	TargetMesh->SetWorldScale3D(FVector(0.5f, 0.5f, 0.05f));

	//设置旋转，让靶子垂直地面（X 轴向上）
	TargetMesh->SetRelativeRotation(FRotator(90.f, 0.f, 0.f));

	// 设置材质：引擎自带的 Basic Wall
	static ConstructorHelpers::FObjectFinder<UMaterial> MaterialAsset(TEXT("/Engine/BasicShapes/BasicShapeMaterial"));
	if (MaterialAsset.Succeeded())
	{
		TargetMesh->SetMaterial(0, MaterialAsset.Object);
	}

	// 启用碰撞
	TargetMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	TargetMesh->SetCollisionObjectType(ECC_WorldDynamic);
	TargetMesh->SetCollisionResponseToAllChannels(ECR_Block);

	// 初始状态
	bIsFalling = false;
	FallSpeed = 100.0f;
}

// Called when the game starts or when spawned
void ATargetActor::BeginPlay()
{
	Super::BeginPlay();
	UE_LOG(LogTemp, Warning, TEXT("BeginPlay TargetMesh forward: %s"), *TargetMesh->GetForwardVector().ToString());
	UE_LOG(LogTemp, Warning, TEXT("BeginPlay TargetMesh up: %s"), *TargetMesh->GetUpVector().ToString());
}

// Called every frame
void ATargetActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (bIsFalling)
	{
		// 旋转倒下
		FRotator NewRotation = TargetMesh->GetRelativeRotation();
		NewRotation.Pitch += FallSpeed * DeltaTime;


		// 简单停止条件
		if (NewRotation.Pitch >= 90.0f)
		{
			NewRotation.Pitch = 90.0f;
			bIsFalling = false;
			UE_LOG(LogTemp, Warning, TEXT("Target finished falling at Pitch: %.2f"), NewRotation.Pitch);
		}
		TargetMesh->SetRelativeRotation(NewRotation);
		UE_LOG(LogTemp, Warning, TEXT("Target is falling... Pitch: %.2f"), NewRotation.Pitch);
	}
}
void ATargetActor::OnHitByBullet()
{
	if (!bIsFalling)
	{
		bIsFalling = true;
	}
}
