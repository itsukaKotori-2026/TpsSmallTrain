//在“项目设置”的“描述”页面中填写您的版权声明。

//引入 TargetActor.h 头文件，确保这个.cpp 文件知道类和成员变量的声明。
#include "ATargetActor.h"
#include "UObject/ConstructorHelpers.h"

// 构造函数
AATargetActor::AATargetActor()
{
 	// 表示这个 Actor 启用了 Tick，每一帧都会调用 Tick 函数。
	PrimaryActorTick.bCanEverTick = true;

	/*创建一个 静态网格体组件 作为靶子的 3D 模型。
	把它设置为 根组件，所以它决定了 Actor 的位置、旋转、缩放。*/
	TargetMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TargetMesh"));
	RootComponent = TargetMesh;

	// 自动加载引擎内置 Cylinder 模型，如果加载成功，将其赋给 TargetMesh。
	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset(TEXT("/Engine/BasicShapes/Cylinder.Cylinder"));
	if (MeshAsset.Succeeded())
	{
		TargetMesh->SetStaticMesh(MeshAsset.Object);
	}
	// 自动加载引擎内置材质，如果加载成功，将其赋给 TargetMesh。
	static ConstructorHelpers::FObjectFinder<UMaterial> MaterialAsset(TEXT("/Engine/BasicShapes/BasicShapeMaterial"));
	if (MaterialAsset.Succeeded())
	{
		TargetMesh->SetMaterial(0, MaterialAsset.Object);
	}
	// 设置缩放，像个靶子的圆盘
	TargetMesh->SetWorldScale3D(FVector(0.5f, 0.5f, 0.05f));
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

//  游戏开始时被调用，这里没有额外逻辑，只是调用基类 BeginPlay()。
void AATargetActor::BeginPlay()
{
	Super::BeginPlay();
	UE_LOG(LogTemp, Warning, TEXT("BeginPlay TargetMesh forward: %s"), *TargetMesh->GetForwardVector().ToString());
	UE_LOG(LogTemp, Warning, TEXT("BeginPlay TargetMesh up: %s"), *TargetMesh->GetUpVector().ToString());
	
}

// 每帧更新靶子的状态，实现被击中后的倒下动画。
void AATargetActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
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
		UE_LOG(LogTemp, Warning, TEXT("Target is falling... Roll: %.2f"), NewRotation.Roll);

	}
}
//当靶子被子弹击中时调用，启动倒下动画。由外部（如角色的 OnFire() 射线检测）调用。
void AATargetActor::OnHitByBullet()
{
	if (!bIsFalling) //仅当靶子未倒下时（!bIsFalling）才会触发。
	{
		bIsFalling = true; // 触发倒下
		UE_LOG(LogTemp, Warning, TEXT("Target starts falling..."));
	}
}

