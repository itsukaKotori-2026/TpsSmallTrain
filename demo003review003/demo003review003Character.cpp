// Copyright Epic Games, Inc. All Rights Reserved.

#include "demo003review003Character.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
//myadd
#include "DrawDebugHelpers.h"
#include "TargetActor.h"
//////////////////////////////////////////////////////////////////////////
// Ademo003review003Character
DEFINE_LOG_CATEGORY_STATIC(LogMyChar, Log, All);
Ademo003review003Character::Ademo003review003Character()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller
	// 调整相机臂的Socket偏移，让角色偏左偏下
	CameraBoom->SocketOffset = FVector(0.f, 50.f, 50.f);
	// 解释：X=0，Y=50往右偏（摄像机视角角色看起来偏左），Z=50往上偏（摄像机略往下看角色）
	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm
	bIsAiming = false;
	PrimaryActorTick.bCanEverTick = true; // 开启 Tick 调用
	UE_LOG(LogMyChar, Log, TEXT("角色构造完成：初始化成功"));
	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)
}

//////////////////////////////////////////////////////////////////////////
// Input

void Ademo003review003Character::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAxis("MoveForward", this, &Ademo003review003Character::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &Ademo003review003Character::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &Ademo003review003Character::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &Ademo003review003Character::LookUpAtRate);

	// handle touch devices
	PlayerInputComponent->BindTouch(IE_Pressed, this, &Ademo003review003Character::TouchStarted);
	PlayerInputComponent->BindTouch(IE_Released, this, &Ademo003review003Character::TouchStopped);

	// VR headset functionality
	PlayerInputComponent->BindAction("ResetVR", IE_Pressed, this, &Ademo003review003Character::OnResetVR);

	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &Ademo003review003Character::OnFire);

	// 瞄准输入：右键按下/松开
	PlayerInputComponent->BindAction("Aim", IE_Pressed, this, &Ademo003review003Character::StartAiming);
	PlayerInputComponent->BindAction("Aim", IE_Released, this, &Ademo003review003Character::StopAiming);

	UE_LOG(LogMyChar, Log, TEXT("输入组件绑定完成"));
}


void Ademo003review003Character::OnResetVR()
{
	// If demo003review003 is added to a project via 'Add Feature' in the Unreal Editor the dependency on HeadMountedDisplay in demo003review003.Build.cs is not automatically propagated
	// and a linker error will result.
	// You will need to either:
	//		Add "HeadMountedDisplay" to [YourProject].Build.cs PublicDependencyModuleNames in order to build successfully (appropriate if supporting VR).
	// or:
	//		Comment or delete the call to ResetOrientationAndPosition below (appropriate if not supporting VR)
	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
}

void Ademo003review003Character::TouchStarted(ETouchIndex::Type FingerIndex, FVector Location)
{
		Jump();
}

void Ademo003review003Character::TouchStopped(ETouchIndex::Type FingerIndex, FVector Location)
{
		StopJumping();
}

void Ademo003review003Character::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void Ademo003review003Character::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void Ademo003review003Character::MoveForward(float Value)
{
	if ((Controller != nullptr) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void Ademo003review003Character::MoveRight(float Value)
{
	if ( (Controller != nullptr) && (Value != 0.0f) )
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
	
		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}
void Ademo003review003Character::OnFire()
{
	UE_LOG(LogTemp, Warning, TEXT("OnFire() start from class: %s"), *GetClass()->GetName());

	// 计算射线起点和终点
	FVector Start = FollowCamera->GetComponentLocation();
	FVector ForwardVector = FollowCamera->GetForwardVector();
	FVector End = Start + (ForwardVector * 10000.0f);

	FHitResult HitResult;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this); // 忽略自身

	// 执行射线检测
	if (GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility, Params))
	{
		if (HitResult.Actor.IsValid())
		{
			UE_LOG(LogTemp, Warning, TEXT("Hit TargetActor类型的对象: %s"), *HitResult.Actor->GetName());

			// 检查是否命中目标Actor
			ATargetActor* Target = Cast<ATargetActor>(HitResult.Actor.Get());
			if (Target)
			{
				Target->OnHitByBullet(); // 调用目标的被击中函数
			}
		}
	}

	// 绘制调试线
	DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, 1.0f, 0, 1.0f);
	UE_LOG(LogTemp, Warning, TEXT("OnFire() end from class: %s"), *GetClass()->GetName());
}
void Ademo003review003Character::StartAiming()
{
	bIsAiming = true;
	CameraBoom->TargetArmLength = 150.f;  // 拉近摄像机
	CameraBoom->SocketOffset = FVector(0.f, 50.f, 10.f);  // 偏右肩

	UE_LOG(LogMyChar, Log, TEXT("进入瞄准状态"));
}

void Ademo003review003Character::StopAiming()
{
	bIsAiming = false;
	CameraBoom->TargetArmLength = 300.f;  // 恢复默认
	CameraBoom->SocketOffset = FVector::ZeroVector;

	UE_LOG(LogMyChar, Log, TEXT("退出瞄准状态"));
}
void Ademo003review003Character::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	float TargetLength = bIsAiming ? 150.f : 300.f;
	CameraBoom->TargetArmLength = FMath::FInterpTo(CameraBoom->TargetArmLength, TargetLength, DeltaTime, 10.f);

	//  默认偏移
	FVector DefaultOffset = FVector(0.f, 50.f, 50.f);
	FVector TargetOffset = bIsAiming ? FVector(0.f, 50.f, 10.f) : DefaultOffset;
	CameraBoom->SocketOffset = FMath::VInterpTo(CameraBoom->SocketOffset, TargetOffset, DeltaTime, 10.f);

	UE_LOG(LogMyChar, Verbose, TEXT("Tick: ArmLength=%.2f, Offset=(%.2f, %.2f, %.2f)"),
		CameraBoom->TargetArmLength,
		CameraBoom->SocketOffset.X,
		CameraBoom->SocketOffset.Y,
		CameraBoom->SocketOffset.Z);

}