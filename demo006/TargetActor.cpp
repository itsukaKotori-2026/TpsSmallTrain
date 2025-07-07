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
	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset(TEXT("/Engine/BasicShapes/Cylinder.Cylinder"));
	if (MeshAsset.Succeeded())
	{
		UE_LOG(LogTemp, Warning, TEXT("Mesh loaded successfully."));
		TargetMesh->SetStaticMesh(MeshAsset.Object);
	}
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
	
}

// Called every frame
void ATargetActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (bIsFalling) {
		FRotator NewRotation = TargetMesh->GetRelativeRotation();
		NewRotation.Roll -= FallSpeed * DeltaTime;

		// 到达目标角度（水平），就停止
		if (NewRotation.Roll <= 0.0f)
		{
			NewRotation.Roll = 0.0f;
			bIsFalling = false;
			UE_LOG(LogTemp, Warning, TEXT("Target finished falling."));
		}

		TargetMesh->SetRelativeRotation(NewRotation);

		UE_LOG(LogTemp, Warning, TEXT("Target is falling... Current Roll: %.2f"), NewRotation.Roll);
		//NewRotation.Roll += FallSpeed * DeltaTime;

		//if (NewRotation.Roll >= 0.0f)
		//{
		//	NewRotation.Roll = 0.0f;
		//	bIsFalling = false;
		//}

		//TargetMesh->SetRelativeRotation(NewRotation);
		//UE_LOG(LogTemp, Warning, TEXT("Target is falling... Current roll: %.2f"), NewRotation.Roll);
	}
}


void ATargetActor::OnHitByBullet()
{
	if (!bIsFalling)
	{
		bIsFalling = true;
	}
}
