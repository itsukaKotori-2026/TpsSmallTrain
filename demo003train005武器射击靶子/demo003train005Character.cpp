// Copyright Epic Games, Inc. All Rights Reserved.

#include "demo003train005Character.h"
#include "AWeapon.h" 
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "DrawDebugHelpers.h"
#include "TargetActor.h"

//////////////////////////////////////////////////////////////////////////
// Ademo003train005Character

Ademo003train005Character::Ademo003train005Character()
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
	bIsAiming = false;
	PrimaryActorTick.bCanEverTick = true; // 开启 Tick 调用
	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)

	

}

//////////////////////////////////////////////////////////////////////////
// Input

void Ademo003train005Character::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAxis("MoveForward", this, &Ademo003train005Character::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &Ademo003train005Character::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &Ademo003train005Character::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &Ademo003train005Character::LookUpAtRate);

	// handle touch devices
	PlayerInputComponent->BindTouch(IE_Pressed, this, &Ademo003train005Character::TouchStarted);
	PlayerInputComponent->BindTouch(IE_Released, this, &Ademo003train005Character::TouchStopped);

	// VR headset functionality
	PlayerInputComponent->BindAction("ResetVR", IE_Pressed, this, &Ademo003train005Character::OnResetVR);
	PlayerInputComponent->BindAction("ResetVR", IE_Pressed, this, &Ademo003train005Character::OnResetVR);

	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &Ademo003train005Character::OnFire);

	// 瞄准输入：右键按下/松开
	PlayerInputComponent->BindAction("Aim", IE_Pressed, this, &Ademo003train005Character::StartAiming);
	PlayerInputComponent->BindAction("Aim", IE_Released, this, &Ademo003train005Character::StopAiming);


}


void Ademo003train005Character::OnResetVR()
{
	// If demo003train005 is added to a project via 'Add Feature' in the Unreal Editor the dependency on HeadMountedDisplay in demo003train005.Build.cs is not automatically propagated
	// and a linker error will result.
	// You will need to either:
	//		Add "HeadMountedDisplay" to [YourProject].Build.cs PublicDependencyModuleNames in order to build successfully (appropriate if supporting VR).
	// or:
	//		Comment or delete the call to ResetOrientationAndPosition below (appropriate if not supporting VR)
	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
}

void Ademo003train005Character::TouchStarted(ETouchIndex::Type FingerIndex, FVector Location)
{
		Jump();
}

void Ademo003train005Character::TouchStopped(ETouchIndex::Type FingerIndex, FVector Location)
{
		StopJumping();
}

void Ademo003train005Character::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void Ademo003train005Character::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void Ademo003train005Character::MoveForward(float Value)
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

void Ademo003train005Character::MoveRight(float Value)
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
void Ademo003train005Character::OnFire()
{
	UE_LOG(LogTemp, Warning, TEXT("OnFire() start , class: %s"), *GetClass()->GetName());
	if (EquippedWeapon)
	{
		EquippedWeapon->Fire();
	}


	UE_LOG(LogTemp, Warning, TEXT("OnFire() end , class: %s"), *GetClass()->GetName());
}
void Ademo003train005Character::StartAiming()
{
	bIsAiming = true;
	CameraBoom->TargetArmLength = 150.f;  // 拉近摄像机
	CameraBoom->SocketOffset = FVector(0.f, 50.f, 10.f);  // 偏右肩


}

void Ademo003train005Character::StopAiming()
{
	bIsAiming = false;
	CameraBoom->TargetArmLength = 300.f;  // 恢复默认
	CameraBoom->SocketOffset = FVector::ZeroVector;


}
void Ademo003train005Character::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	float TargetLength = bIsAiming ? 150.f : 300.f;
	CameraBoom->TargetArmLength = FMath::FInterpTo(CameraBoom->TargetArmLength, TargetLength, DeltaTime, 10.f);

	//  默认偏移
	FVector DefaultOffset = FVector(0.f, 50.f, 50.f);
	FVector TargetOffset = bIsAiming ? FVector(0.f, 50.f, 10.f) : DefaultOffset;
	CameraBoom->SocketOffset = FMath::VInterpTo(CameraBoom->SocketOffset, TargetOffset, DeltaTime, 10.f);

	if (EquippedWeapon)
	{
		FVector WeaponLocation = EquippedWeapon->GetActorLocation();
		UE_LOG(LogTemp, Warning, TEXT("Weapon location during tick: %s"), *WeaponLocation.ToString());
	}
}
void Ademo003train005Character::BeginPlay()
{
	Super::BeginPlay();
	// 假设 AAWeapon 是你的武器基类，AWeaponClass 是一个继承自 AAWeapon 的子类
	DefaultWeaponClass = AAWeapon::StaticClass();
	if (DefaultWeaponClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("Successfully set DefaultWeaponClass to: %s"), *DefaultWeaponClass->GetName());
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to set DefaultWeaponClass!"));
	}
	if (DefaultWeaponClass)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		SpawnParams.Instigator = this;

		EquippedWeapon = GetWorld()->SpawnActor<AAWeapon>(DefaultWeaponClass, SpawnParams);

		if (EquippedWeapon)
		{
			UE_LOG(LogTemp, Warning, TEXT("Weapon successfully spawned at location: %s"), *EquippedWeapon->GetActorLocation().ToString());

			EquippedWeapon->AttachToComponent(
				GetMesh(),
				FAttachmentTransformRules::SnapToTargetNotIncludingScale,
				FName("WeaponSocket") // Socket 名必须提前在骨骼资源里建好
			);

			// 输出武器位置，确认是否正确附加
			FVector WeaponLocation = EquippedWeapon->GetActorLocation();
			UE_LOG(LogTemp, Warning, TEXT("Weapon attached at: %s"), *WeaponLocation.ToString());
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Failed to create EquippedWeapon!"));
		}
	}
}