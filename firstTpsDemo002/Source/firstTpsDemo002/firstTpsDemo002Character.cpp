// 版权所有Epic Games，股份有限公司保留所有权利。

#include "firstTpsDemo002Character.h"
#include "ATargetActor.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "DrawDebugHelpers.h"


//////////////////////////////////////////////////////////////////////////
// AfirstTpsDemo002Character

AfirstTpsDemo002Character::AfirstTpsDemo002Character()
{
	// 设置碰撞舱的尺寸
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// 设置我们的输入周转率
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// 控制器旋转时不要旋转。让它只影响相机。
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// 配置角色移动
	GetCharacterMovement()->bOrientRotationToMovement = true; // 字符沿输入方向移动。	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...在这个转速下
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	// 创建一个摄像头吊杆（如果发生碰撞，将其拉向玩家）
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // 摄影机跟在角色后面这么远的地方
	CameraBoom->bUsePawnControlRotation = true; // 根据控制器旋转手臂

	// 创建跟踪摄像头
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // 将摄像头连接到吊杆末端，让吊杆调整以匹配控制器方向
	FollowCamera->bUsePawnControlRotation = false; //相机不相对于手臂旋转

	// 注意：网格组件上的骨架网格和动画蓝图引用（继承自角色）
	// 在名为MyCharacter的派生蓝图资产中设置（以避免在C++中直接引用内容）
}

//////////////////////////////////////////////////////////////////////////
// 输入

void AfirstTpsDemo002Character::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// 设置游戏密钥绑定
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAxis("MoveForward", this, &AfirstTpsDemo002Character::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AfirstTpsDemo002Character::MoveRight);

	// 我们有两个版本的旋转绑定，可以不同地处理不同类型的设备
	// turn”控制提供绝对增量的设备，如鼠标。
	// “周转率”是指我们选择将其视为变化率的设备，例如模拟操纵杆
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &AfirstTpsDemo002Character::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AfirstTpsDemo002Character::LookUpAtRate);

	// 手柄触摸装置
	PlayerInputComponent->BindTouch(IE_Pressed, this, &AfirstTpsDemo002Character::TouchStarted);
	PlayerInputComponent->BindTouch(IE_Released, this, &AfirstTpsDemo002Character::TouchStopped);

	// VR耳机功能
	PlayerInputComponent->BindAction("ResetVR", IE_Pressed, this, &AfirstTpsDemo002Character::OnResetVR);

	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &AfirstTpsDemo002Character::OnFire);

}


void AfirstTpsDemo002Character::OnResetVR()
{
	// //如果通过虚幻编辑器中的“添加功能”将firstTpsDemo002添加到项目中，则不会自动传播firstTpsDomo002.Build.cs中对HeadMountedDisplay的依赖关系并且将导致链接器错误。
	// 您需要：将“HeadMountedDisplay”添加到[YourProject]。Build.cs PublicDependencyModuleNames，以便成功构建（如果支持VR，则适用）。
	// 或者：注释或删除下面对ResetPorientationAndPosition的调用（如果不支持VR，则适用）

	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
}

void AfirstTpsDemo002Character::TouchStarted(ETouchIndex::Type FingerIndex, FVector Location)
{
		Jump();
}

void AfirstTpsDemo002Character::TouchStopped(ETouchIndex::Type FingerIndex, FVector Location)
{
		StopJumping();
}

void AfirstTpsDemo002Character::TurnAtRate(float Rate)
{
	// 根据速率信息计算此帧的增量
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AfirstTpsDemo002Character::LookUpAtRate(float Rate)
{
	// 根据速率信息计算此帧的增量
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void AfirstTpsDemo002Character::MoveForward(float Value)
{
	if ((Controller != nullptr) && (Value != 0.0f))
	{
		// 找出前进的方向
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// 获取正向向量
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void AfirstTpsDemo002Character::MoveRight(float Value)
{
	if ( (Controller != nullptr) && (Value != 0.0f) )
	{
		// 找出哪条路是正确的
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
	
		// 获得正确的向量
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// 增加该方向的运动
		AddMovementInput(Direction, Value);
	}
}

void AfirstTpsDemo002Character::OnFire()
{
	UE_LOG(LogTemp, Warning, TEXT("OnFire called!"));

	// 射线起点：摄像机位置
	FVector Start = FollowCamera->GetComponentLocation();
	// 射线方向：摄像机前方
	FVector ForwardVector = FollowCamera->GetForwardVector();
	// 射线终点：相机前方10000单位
	FVector End = Start + (ForwardVector * 10000.0f);

	FHitResult HitResult;//存储射线检测的结果信息
	FCollisionQueryParams Params;//配置检测参数
	Params.AddIgnoredActor(this);// 忽略自身
	//执行实际的射线检测，LineTraceSingleByChannel：沿直线检测碰撞，ECC_Visibility：使用Visibility碰撞通道
	//	返回bool值表示是否命中物体
	if (GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility, Params))
	{ //处理命中的物体，首先验证命中的Actor是否有效
		if (HitResult.Actor.IsValid())
		{
			//输出命中物体的名称（调试用）
			UE_LOG(LogTemp, Warning, TEXT("Hit: %s"), *HitResult.Actor->GetName());

			// 判断是否是靶子，尝试将命中的Actor转换为目标类型AATargetActor
			AATargetActor* Target = Cast<AATargetActor>(HitResult.Actor.Get());
			//如果是目标类型，调用其OnHitByBullet()方法
			if (Target)
			{
				Target->OnHitByBullet();
			}
		}
	}

	// 调试线
	DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, 1.0f, 0, 1.0f);
}
