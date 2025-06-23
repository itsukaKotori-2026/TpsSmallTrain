// ��Ȩ����Epic Games���ɷ����޹�˾��������Ȩ����

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
	// ������ײ�յĳߴ�
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// �������ǵ�������ת��
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// ��������תʱ��Ҫ��ת������ֻӰ�������
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// ���ý�ɫ�ƶ�
	GetCharacterMovement()->bOrientRotationToMovement = true; // �ַ������뷽���ƶ���	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...�����ת����
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	// ����һ������ͷ���ˣ����������ײ������������ң�
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // ��Ӱ�����ڽ�ɫ������ôԶ�ĵط�
	CameraBoom->bUsePawnControlRotation = true; // ���ݿ�������ת�ֱ�

	// ������������ͷ
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // ������ͷ���ӵ�����ĩ�ˣ��õ��˵�����ƥ�����������
	FollowCamera->bUsePawnControlRotation = false; //�����������ֱ���ת

	// ע�⣺��������ϵĹǼ�����Ͷ�����ͼ���ã��̳��Խ�ɫ��
	// ����ΪMyCharacter��������ͼ�ʲ������ã��Ա�����C++��ֱ���������ݣ�
}

//////////////////////////////////////////////////////////////////////////
// ����

void AfirstTpsDemo002Character::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// ������Ϸ��Կ��
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAxis("MoveForward", this, &AfirstTpsDemo002Character::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AfirstTpsDemo002Character::MoveRight);

	// �����������汾����ת�󶨣����Բ�ͬ�ش���ͬ���͵��豸
	// turn�������ṩ�����������豸������ꡣ
	// ����ת�ʡ���ָ����ѡ������Ϊ�仯�ʵ��豸������ģ����ݸ�
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &AfirstTpsDemo002Character::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AfirstTpsDemo002Character::LookUpAtRate);

	// �ֱ�����װ��
	PlayerInputComponent->BindTouch(IE_Pressed, this, &AfirstTpsDemo002Character::TouchStarted);
	PlayerInputComponent->BindTouch(IE_Released, this, &AfirstTpsDemo002Character::TouchStopped);

	// VR��������
	PlayerInputComponent->BindAction("ResetVR", IE_Pressed, this, &AfirstTpsDemo002Character::OnResetVR);

	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &AfirstTpsDemo002Character::OnFire);

}


void AfirstTpsDemo002Character::OnResetVR()
{
	// //���ͨ����ñ༭���еġ���ӹ��ܡ���firstTpsDemo002��ӵ���Ŀ�У��򲻻��Զ�����firstTpsDomo002.Build.cs�ж�HeadMountedDisplay��������ϵ���ҽ���������������
	// ����Ҫ������HeadMountedDisplay����ӵ�[YourProject]��Build.cs PublicDependencyModuleNames���Ա�ɹ����������֧��VR�������ã���
	// ���ߣ�ע�ͻ�ɾ�������ResetPorientationAndPosition�ĵ��ã������֧��VR�������ã�

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
	// ����������Ϣ�����֡������
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AfirstTpsDemo002Character::LookUpAtRate(float Rate)
{
	// ����������Ϣ�����֡������
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void AfirstTpsDemo002Character::MoveForward(float Value)
{
	if ((Controller != nullptr) && (Value != 0.0f))
	{
		// �ҳ�ǰ���ķ���
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// ��ȡ��������
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void AfirstTpsDemo002Character::MoveRight(float Value)
{
	if ( (Controller != nullptr) && (Value != 0.0f) )
	{
		// �ҳ�����·����ȷ��
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
	
		// �����ȷ������
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// ���Ӹ÷�����˶�
		AddMovementInput(Direction, Value);
	}
}

void AfirstTpsDemo002Character::OnFire()
{
	UE_LOG(LogTemp, Warning, TEXT("OnFire called!"));

	// ������㣺�����λ��
	FVector Start = FollowCamera->GetComponentLocation();
	// ���߷��������ǰ��
	FVector ForwardVector = FollowCamera->GetForwardVector();
	// �����յ㣺���ǰ��10000��λ
	FVector End = Start + (ForwardVector * 10000.0f);

	FHitResult HitResult;//�洢���߼��Ľ����Ϣ
	FCollisionQueryParams Params;//���ü�����
	Params.AddIgnoredActor(this);// ��������
	//ִ��ʵ�ʵ����߼�⣬LineTraceSingleByChannel����ֱ�߼����ײ��ECC_Visibility��ʹ��Visibility��ײͨ��
	//	����boolֵ��ʾ�Ƿ���������
	if (GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility, Params))
	{ //�������е����壬������֤���е�Actor�Ƿ���Ч
		if (HitResult.Actor.IsValid())
		{
			//���������������ƣ������ã�
			UE_LOG(LogTemp, Warning, TEXT("Hit: %s"), *HitResult.Actor->GetName());

			// �ж��Ƿ��ǰ��ӣ����Խ����е�Actorת��ΪĿ������AATargetActor
			AATargetActor* Target = Cast<AATargetActor>(HitResult.Actor.Get());
			//�����Ŀ�����ͣ�������OnHitByBullet()����
			if (Target)
			{
				Target->OnHitByBullet();
			}
		}
	}

	// ������
	DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, 1.0f, 0, 1.0f);
}
