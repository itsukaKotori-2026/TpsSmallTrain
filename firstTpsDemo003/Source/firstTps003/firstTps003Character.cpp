// Copyright Epic Games, Inc. All Rights Reserved.

#include "firstTps003Character.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"

//////////////////////////////////////////////////////////////////////////
// AfirstTps003Character
// 日志宏定义，可选
DEFINE_LOG_CATEGORY_STATIC(LogMyChar, Log, All);
AfirstTps003Character::AfirstTps003Character()
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

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)
	// 原有初始化...
	bIsAiming = false;
	PrimaryActorTick.bCanEverTick = true; // 开启 Tick 调用
	UE_LOG(LogMyChar, Log, TEXT("角色构造完成：初始化成功"));
}

//////////////////////////////////////////////////////////////////////////
// Input

void AfirstTps003Character::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAxis("MoveForward", this, &AfirstTps003Character::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AfirstTps003Character::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &AfirstTps003Character::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AfirstTps003Character::LookUpAtRate);

	// handle touch devices
	PlayerInputComponent->BindTouch(IE_Pressed, this, &AfirstTps003Character::TouchStarted);
	PlayerInputComponent->BindTouch(IE_Released, this, &AfirstTps003Character::TouchStopped);

	// VR headset functionality
	PlayerInputComponent->BindAction("ResetVR", IE_Pressed, this, &AfirstTps003Character::OnResetVR);

	// 瞄准输入：右键按下/松开
	PlayerInputComponent->BindAction("Aim", IE_Pressed, this, &AfirstTps003Character::StartAiming);
	PlayerInputComponent->BindAction("Aim", IE_Released, this, &AfirstTps003Character::StopAiming);

	UE_LOG(LogMyChar, Log, TEXT("输入组件绑定完成"));
}


void AfirstTps003Character::OnResetVR()
{
	// If firstTps003 is added to a project via 'Add Feature' in the Unreal Editor the dependency on HeadMountedDisplay in firstTps003.Build.cs is not automatically propagated
	// and a linker error will result.
	// You will need to either:
	//		Add "HeadMountedDisplay" to [YourProject].Build.cs PublicDependencyModuleNames in order to build successfully (appropriate if supporting VR).
	// or:
	//		Comment or delete the call to ResetOrientationAndPosition below (appropriate if not supporting VR)
	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
}

void AfirstTps003Character::TouchStarted(ETouchIndex::Type FingerIndex, FVector Location)
{
		Jump();
}

void AfirstTps003Character::TouchStopped(ETouchIndex::Type FingerIndex, FVector Location)
{
		StopJumping();
}

void AfirstTps003Character::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AfirstTps003Character::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void AfirstTps003Character::MoveForward(float Value)
{
	if ((Controller != nullptr) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);

		UE_LOG(LogMyChar, Log, TEXT("前进/后退输入：值 = %f"), Value);
	}
}

void AfirstTps003Character::MoveRight(float Value)
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

		UE_LOG(LogMyChar, Log, TEXT("左右移动输入：值 = %f"), Value)
	}
}
void AfirstTps003Character::StartAiming()
{
	bIsAiming = true;
	CameraBoom->TargetArmLength = 150.f;  // 拉近摄像机
	CameraBoom->SocketOffset = FVector(0.f, 50.f, 10.f);  // 偏右肩

	UE_LOG(LogMyChar, Log, TEXT("进入瞄准状态"));
}

void AfirstTps003Character::StopAiming()
{
	bIsAiming = false;
	CameraBoom->TargetArmLength = 300.f;  // 恢复默认
	CameraBoom->SocketOffset = FVector::ZeroVector;

	UE_LOG(LogMyChar, Log, TEXT("退出瞄准状态"));
}
void AfirstTps003Character::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	float TargetLength = bIsAiming ? 150.f : 300.f;
	CameraBoom->TargetArmLength = FMath::FInterpTo(CameraBoom->TargetArmLength, TargetLength, DeltaTime, 10.f);

	FVector TargetOffset = bIsAiming ? FVector(0.f, 50.f, 10.f) : FVector::ZeroVector;
	CameraBoom->SocketOffset = FMath::VInterpTo(CameraBoom->SocketOffset, TargetOffset, DeltaTime, 10.f);

	// 输出 Tick 时相机臂长度和偏移
	UE_LOG(LogMyChar, Verbose, TEXT("Tick: ArmLength=%.2f, Offset=(%.2f, %.2f, %.2f)"),
		CameraBoom->TargetArmLength,
		CameraBoom->SocketOffset.X,
		CameraBoom->SocketOffset.Y,
		CameraBoom->SocketOffset.Z);

}
