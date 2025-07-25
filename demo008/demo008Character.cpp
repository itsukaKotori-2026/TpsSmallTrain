// Copyright Epic Games, Inc. All Rights Reserved.

#include "demo008Character.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "ARifle.h"  // 引入 ARifle 头文件
#include "AKA47Weapon.h"
#include "AGrenadeWeapon.h"
#include "MeleeWeapon.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"


//////////////////////////////////////////////////////////////////////////
// Ademo008Character

Ademo008Character::Ademo008Character()
{
	// 设置角色的碰撞大小 InitCapsuleSize
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// 设置角色旋转的速度 BaseTurnRate BaseLookUpRate
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;
	 
	// 不让控制器旋转角色，让控制器只影响相机 bUseControllerRotationPitch bUseControllerRotationYaw bUseControllerRotationRoll
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// 配置角色的旋转速度 RotationRate 跳跃速度 JumpZVelocity 空中控制 AirControl 
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	// 创建相机悬臂 CameraBoom
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	//创建跟随相机 FollowCamera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	bIsAiming = false;
	PrimaryActorTick.bCanEverTick = true; // 允许每帧调用Tick函数
}

//////////////////////////////////////////////////////////////////////////
// Input

void Ademo008Character::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAxis("MoveForward", this, &Ademo008Character::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &Ademo008Character::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &Ademo008Character::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &Ademo008Character::LookUpAtRate);

	// handle touch devices
	PlayerInputComponent->BindTouch(IE_Pressed, this, &Ademo008Character::TouchStarted);
	PlayerInputComponent->BindTouch(IE_Released, this, &Ademo008Character::TouchStopped);

	// VR headset functionality
	PlayerInputComponent->BindAction("ResetVR", IE_Pressed, this, &Ademo008Character::OnResetVR);

	// 绑定射击输入
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &Ademo008Character::StartFire);
	PlayerInputComponent->BindAction("Fire", IE_Released, this, &Ademo008Character::StopFire);

	// 绑定瞄准和重载输入
	PlayerInputComponent->BindAction("Aim", IE_Pressed, this, &Ademo008Character::StartAiming);
	PlayerInputComponent->BindAction("Aim", IE_Released, this, &Ademo008Character::StopAiming);
	PlayerInputComponent->BindAction("Reload", IE_Pressed, this, &Ademo008Character::OnReload);

	// 绑定武器切换输入
	PlayerInputComponent->BindAction("Weapon1", IE_Pressed, this, &Ademo008Character::EquipWeaponSlot1);
	PlayerInputComponent->BindAction("Weapon2", IE_Pressed, this, &Ademo008Character::EquipWeaponSlot2);

	PlayerInputComponent->BindAction("MouseWheelUp", IE_Pressed, this, &Ademo008Character::NextWeapon);
	PlayerInputComponent->BindAction("MouseWheelDown", IE_Pressed, this, &Ademo008Character::PrevWeapon);

	// 绑定切换武器的输入
	PlayerInputComponent->BindAction("Weapon3", IE_Pressed, this, &Ademo008Character::EquipMeleeWeapon);
	PlayerInputComponent->BindAction("Weapon4", IE_Pressed, this, &Ademo008Character::EquipGrenadeWeapon);
}


void Ademo008Character::OnResetVR()
{
	// If demo008 is added to a project via 'Add Feature' in the Unreal Editor the dependency on HeadMountedDisplay in demo008.Build.cs is not automatically propagated
	// and a linker error will result.
	// You will need to either:
	//		Add "HeadMountedDisplay" to [YourProject].Build.cs PublicDependencyModuleNames in order to build successfully (appropriate if supporting VR).
	// or:
	//		Comment or delete the call to ResetOrientationAndPosition below (appropriate if not supporting VR)
	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
}

void Ademo008Character::TouchStarted(ETouchIndex::Type FingerIndex, FVector Location)
{
		Jump();
}

void Ademo008Character::TouchStopped(ETouchIndex::Type FingerIndex, FVector Location)
{
		StopJumping();
}

void Ademo008Character::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void Ademo008Character::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void Ademo008Character::MoveForward(float Value)
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

void Ademo008Character::MoveRight(float Value)
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
void Ademo008Character::BeginPlay()
{
	Super::BeginPlay();
	// 强制设置为 GameOnly 输入模式，确保鼠标滚轮等输入不被拦截
	APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0);
	if (PC)
	{
		FInputModeGameOnly InputMode;
		PC->SetInputMode(InputMode);
		PC->bShowMouseCursor = false;
	}

	// 指定主副武器类型
	DefaultWeaponClass1 = AAKA47Weapon::StaticClass(); // 主武器 AK
	DefaultWeaponClass2 = AARifle::StaticClass();      // 副武器 AR4

	DefaultWeaponClass3 = AMeleeWeapon::StaticClass();      // 
	DefaultWeaponClass4 = AAGrenadeWeapon::StaticClass(); //手雷 
	SetupWeaponInventory(); // 初始化武器列表
}

void Ademo008Character::SetupWeaponInventory()
{
	Inventory.Empty();

	TArray<TSubclassOf<AAWeapon>> WeaponClasses;
	WeaponClasses.Add(DefaultWeaponClass1); // 主武器：AK
	WeaponClasses.Add(DefaultWeaponClass2); // 副武器：AR4
	WeaponClasses.Add(DefaultWeaponClass3);// 近战武器
	WeaponClasses.Add(DefaultWeaponClass4);
	for (int32 i = 0; i < WeaponClasses.Num(); ++i)
	{
		if (!*WeaponClasses[i])
		{
			UE_LOG(LogTemp, Error, TEXT("Weapon class at index %d is invalid!"), i);
			continue;
		}
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		SpawnParams.Instigator = this;

		AAWeapon* NewWeapon = GetWorld()->SpawnActor<AAWeapon>(WeaponClasses[i], SpawnParams);

		if (NewWeapon)
		{
			NewWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName("WeaponSocket"));
			// 对于主武器和副武器，确保它朝向正确
			bool bIsMainOrSecondaryWeapon =
				NewWeapon &&
				(NewWeapon->IsA(AAKA47Weapon::StaticClass()) || NewWeapon->IsA(AARifle::StaticClass()));

			if (bIsMainOrSecondaryWeapon)
			{
				FVector LocationOffset = FVector(0.f, 0.f, 0.f); // 可根据需要调整
				FRotator RotationOffset = FRotator(0.f, 90.f, 0.f); // 让枪横过来
				NewWeapon->SetActorRelativeLocation(LocationOffset);
				NewWeapon->SetActorRelativeRotation(RotationOffset);
			}
			// 对于近战武器（匕首），确保它朝向正确
			bool bIsThirdOrFourthWeapon =
				NewWeapon &&
				(NewWeapon->IsA(AMeleeWeapon::StaticClass()) || NewWeapon->IsA(AAGrenadeWeapon::StaticClass()));
			if (bIsThirdOrFourthWeapon)
			{
				FVector LocationOffset = FVector(-20.f, 0.f, 0.f); // 根据需要微调
				FRotator RotationOffset = FRotator(90.f, 180.f, 0.f); // 朝向反转

				NewWeapon->SetActorRelativeLocation(LocationOffset);
				NewWeapon->SetActorRelativeRotation(RotationOffset);
				UE_LOG(LogTemp, Warning, TEXT("World Rotation of weapon: %s"), *NewWeapon->GetActorRotation().ToString());
				DrawDebugCoordinateSystem(GetWorld(), NewWeapon->GetActorLocation(), NewWeapon->GetActorRotation(), 20.f, true, 10.f, 0, 2.f);
			}
			NewWeapon->SetActorHiddenInGame(true); // 初始隐藏
			Inventory.Add(NewWeapon);
			UE_LOG(LogTemp, Warning, TEXT("Weapon %s attached to character."), *NewWeapon->GetName());
		}
	}

	if (Inventory.Num() > 0)
	{
		EquipWeapon(0); // 默认切到主武器
	}
	UE_LOG(LogTemp, Warning, TEXT("Inventory count: %d"), Inventory.Num());

}

void Ademo008Character::EquipWeapon(int32 Index)
{
	if (Index < 0 || Index >= Inventory.Num() || Index == CurrentWeaponIndex)
		return;

	StopFire();

	if (EquippedWeapon)
	{
		EquippedWeapon->SetActorHiddenInGame(true);
	}

	EquippedWeapon = Inventory[Index];
	CurrentWeaponIndex = Index;

	if (EquippedWeapon)
	{
		EquippedWeapon->SetActorHiddenInGame(false);
		EquippedWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName("WeaponSocket"));
		UE_LOG(LogTemp, Log, TEXT("Equipped weapon: %s"), *EquippedWeapon->GetName());
	}
}
void Ademo008Character::HandleMouseWheel(float Value)
{

	if (Value > 0.f)
	{
		UE_LOG(LogTemp, Warning, TEXT("MouseWheel Value: %f"), Value);
		PrevWeapon();
	}
	else if (Value < 0.f)
	{
		UE_LOG(LogTemp, Warning, TEXT("MouseWheel Value: %f"), Value);
		NextWeapon();
	}
}

void Ademo008Character::NextWeapon()
{
	int32 NewIndex = (CurrentWeaponIndex + 1) % Inventory.Num();
	EquipWeapon(NewIndex);
}

void Ademo008Character::PrevWeapon()
{
	int32 NewIndex = (CurrentWeaponIndex - 1 + Inventory.Num()) % Inventory.Num();
	EquipWeapon(NewIndex);
}

void Ademo008Character::OnFire()
{
	// 判断是否有装备武器
	if (!EquippedWeapon)
	{
		StopFire();
		return;
	}

	// 判断是否正在换弹
	if (EquippedWeapon->IsReloading())
	{
		StopFire();
		return;
	}

	// 判断当前是否还有子弹
	if (EquippedWeapon->GetCurrentAmmo() <= 0)
	{
		StopFire(); // 自动停止开火
		return;
	}

	// 先减少弹药，再开火
	EquippedWeapon->DecreaseAmmo(1);
	EquippedWeapon->Fire();
}
void Ademo008Character::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	float TargetLength = bIsAiming ? 150.f : 300.f;
	CameraBoom->TargetArmLength = FMath::FInterpTo(CameraBoom->TargetArmLength, TargetLength, DeltaTime, 10.f);

	//  默认偏移
	FVector TargetOffset = bIsAiming ? FVector(0.f, 50.f, 10.f) : FVector(0.f, 50.f, 50.f);
	CameraBoom->SocketOffset = FMath::VInterpTo(CameraBoom->SocketOffset, TargetOffset, DeltaTime, 10.f);
	// 准星扩散控制
	if (bIsFiring)
	{
		CrosshairSpread = FMath::Min(CrosshairSpread + SpreadIncreaseRate * DeltaTime, MaxCrosshairSpread);
	}
	else
	{
		CrosshairSpread = FMath::Max(CrosshairSpread - SpreadDecreaseRate * DeltaTime, 0.0f);
	}
}
void Ademo008Character::StartAiming()
{
	bIsAiming = true;
}

void Ademo008Character::StopAiming()
{
	bIsAiming = false;
}

void Ademo008Character::StartFire()
{
	if (bIsFiring || EquippedWeapon == nullptr) return;

	// 判断武器是否正在换弹
	if (EquippedWeapon->IsReloading())
	{
		return;
	}
	bIsFiring = true;

	// 立即开一次火
	OnFire();

	// 开始循环调用 OnFire（每隔 FireRate 秒）
	GetWorld()->GetTimerManager().SetTimer(FireTimerHandle, this, &Ademo008Character::OnFire, FireRate, true);
}

void Ademo008Character::StopFire()
{
	if (!bIsFiring) return;

	bIsFiring = false;

	// 停止定时器
	GetWorld()->GetTimerManager().ClearTimer(FireTimerHandle);
}
void Ademo008Character::OnReload()
{
	if (EquippedWeapon)
	{
		EquippedWeapon->StartReload();
	}
}