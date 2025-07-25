// Copyright Epic Games, Inc. All Rights Reserved.

#include "demo008Character.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "ARifle.h"  // ���� ARifle ͷ�ļ�
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
	// ���ý�ɫ����ײ��С InitCapsuleSize
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// ���ý�ɫ��ת���ٶ� BaseTurnRate BaseLookUpRate
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;
	 
	// ���ÿ�������ת��ɫ���ÿ�����ֻӰ����� bUseControllerRotationPitch bUseControllerRotationYaw bUseControllerRotationRoll
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// ���ý�ɫ����ת�ٶ� RotationRate ��Ծ�ٶ� JumpZVelocity ���п��� AirControl 
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	// ����������� CameraBoom
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	//����������� FollowCamera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	bIsAiming = false;
	PrimaryActorTick.bCanEverTick = true; // ����ÿ֡����Tick����
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

	// ���������
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &Ademo008Character::StartFire);
	PlayerInputComponent->BindAction("Fire", IE_Released, this, &Ademo008Character::StopFire);

	// ����׼����������
	PlayerInputComponent->BindAction("Aim", IE_Pressed, this, &Ademo008Character::StartAiming);
	PlayerInputComponent->BindAction("Aim", IE_Released, this, &Ademo008Character::StopAiming);
	PlayerInputComponent->BindAction("Reload", IE_Pressed, this, &Ademo008Character::OnReload);

	// �������л�����
	PlayerInputComponent->BindAction("Weapon1", IE_Pressed, this, &Ademo008Character::EquipWeaponSlot1);
	PlayerInputComponent->BindAction("Weapon2", IE_Pressed, this, &Ademo008Character::EquipWeaponSlot2);

	PlayerInputComponent->BindAction("MouseWheelUp", IE_Pressed, this, &Ademo008Character::NextWeapon);
	PlayerInputComponent->BindAction("MouseWheelDown", IE_Pressed, this, &Ademo008Character::PrevWeapon);

	// ���л�����������
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
	// ǿ������Ϊ GameOnly ����ģʽ��ȷ�������ֵ����벻������
	APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0);
	if (PC)
	{
		FInputModeGameOnly InputMode;
		PC->SetInputMode(InputMode);
		PC->bShowMouseCursor = false;
	}

	// ָ��������������
	DefaultWeaponClass1 = AAKA47Weapon::StaticClass(); // ������ AK
	DefaultWeaponClass2 = AARifle::StaticClass();      // ������ AR4

	DefaultWeaponClass3 = AMeleeWeapon::StaticClass();      // 
	DefaultWeaponClass4 = AAGrenadeWeapon::StaticClass(); //���� 
	SetupWeaponInventory(); // ��ʼ�������б�
}

void Ademo008Character::SetupWeaponInventory()
{
	Inventory.Empty();

	TArray<TSubclassOf<AAWeapon>> WeaponClasses;
	WeaponClasses.Add(DefaultWeaponClass1); // ��������AK
	WeaponClasses.Add(DefaultWeaponClass2); // ��������AR4
	WeaponClasses.Add(DefaultWeaponClass3);// ��ս����
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
			// �����������͸�������ȷ����������ȷ
			bool bIsMainOrSecondaryWeapon =
				NewWeapon &&
				(NewWeapon->IsA(AAKA47Weapon::StaticClass()) || NewWeapon->IsA(AARifle::StaticClass()));

			if (bIsMainOrSecondaryWeapon)
			{
				FVector LocationOffset = FVector(0.f, 0.f, 0.f); // �ɸ�����Ҫ����
				FRotator RotationOffset = FRotator(0.f, 90.f, 0.f); // ��ǹ�����
				NewWeapon->SetActorRelativeLocation(LocationOffset);
				NewWeapon->SetActorRelativeRotation(RotationOffset);
			}
			// ���ڽ�ս������ذ�ף���ȷ����������ȷ
			bool bIsThirdOrFourthWeapon =
				NewWeapon &&
				(NewWeapon->IsA(AMeleeWeapon::StaticClass()) || NewWeapon->IsA(AAGrenadeWeapon::StaticClass()));
			if (bIsThirdOrFourthWeapon)
			{
				FVector LocationOffset = FVector(-20.f, 0.f, 0.f); // ������Ҫ΢��
				FRotator RotationOffset = FRotator(90.f, 180.f, 0.f); // ����ת

				NewWeapon->SetActorRelativeLocation(LocationOffset);
				NewWeapon->SetActorRelativeRotation(RotationOffset);
				UE_LOG(LogTemp, Warning, TEXT("World Rotation of weapon: %s"), *NewWeapon->GetActorRotation().ToString());
				DrawDebugCoordinateSystem(GetWorld(), NewWeapon->GetActorLocation(), NewWeapon->GetActorRotation(), 20.f, true, 10.f, 0, 2.f);
			}
			NewWeapon->SetActorHiddenInGame(true); // ��ʼ����
			Inventory.Add(NewWeapon);
			UE_LOG(LogTemp, Warning, TEXT("Weapon %s attached to character."), *NewWeapon->GetName());
		}
	}

	if (Inventory.Num() > 0)
	{
		EquipWeapon(0); // Ĭ���е�������
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
	// �ж��Ƿ���װ������
	if (!EquippedWeapon)
	{
		StopFire();
		return;
	}

	// �ж��Ƿ����ڻ���
	if (EquippedWeapon->IsReloading())
	{
		StopFire();
		return;
	}

	// �жϵ�ǰ�Ƿ����ӵ�
	if (EquippedWeapon->GetCurrentAmmo() <= 0)
	{
		StopFire(); // �Զ�ֹͣ����
		return;
	}

	// �ȼ��ٵ�ҩ���ٿ���
	EquippedWeapon->DecreaseAmmo(1);
	EquippedWeapon->Fire();
}
void Ademo008Character::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	float TargetLength = bIsAiming ? 150.f : 300.f;
	CameraBoom->TargetArmLength = FMath::FInterpTo(CameraBoom->TargetArmLength, TargetLength, DeltaTime, 10.f);

	//  Ĭ��ƫ��
	FVector TargetOffset = bIsAiming ? FVector(0.f, 50.f, 10.f) : FVector(0.f, 50.f, 50.f);
	CameraBoom->SocketOffset = FMath::VInterpTo(CameraBoom->SocketOffset, TargetOffset, DeltaTime, 10.f);
	// ׼����ɢ����
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

	// �ж������Ƿ����ڻ���
	if (EquippedWeapon->IsReloading())
	{
		return;
	}
	bIsFiring = true;

	// ������һ�λ�
	OnFire();

	// ��ʼѭ������ OnFire��ÿ�� FireRate �룩
	GetWorld()->GetTimerManager().SetTimer(FireTimerHandle, this, &Ademo008Character::OnFire, FireRate, true);
}

void Ademo008Character::StopFire()
{
	if (!bIsFiring) return;

	bIsFiring = false;

	// ֹͣ��ʱ��
	GetWorld()->GetTimerManager().ClearTimer(FireTimerHandle);
}
void Ademo008Character::OnReload()
{
	if (EquippedWeapon)
	{
		EquippedWeapon->StartReload();
	}
}