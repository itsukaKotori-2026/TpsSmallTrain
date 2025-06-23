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

	// ����һ�������Դ�ģ�ͣ�Cylinder
	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset(TEXT("/Engine/BasicShapes/Cylinder.Cylinder"));
	if (MeshAsset.Succeeded())
	{
		TargetMesh->SetStaticMesh(MeshAsset.Object);
	}

	// �������ţ�����Բ������
	TargetMesh->SetWorldScale3D(FVector(0.5f, 0.5f, 0.05f));

	//������ת���ð��Ӵ�ֱ���棨X �����ϣ�
	TargetMesh->SetRelativeRotation(FRotator(90.f, 0.f, 0.f));

	// ���ò��ʣ������Դ��� Basic Wall
	static ConstructorHelpers::FObjectFinder<UMaterial> MaterialAsset(TEXT("/Engine/BasicShapes/BasicShapeMaterial"));
	if (MaterialAsset.Succeeded())
	{
		TargetMesh->SetMaterial(0, MaterialAsset.Object);
	}

	// ������ײ
	TargetMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	TargetMesh->SetCollisionObjectType(ECC_WorldDynamic);
	TargetMesh->SetCollisionResponseToAllChannels(ECR_Block);

	// ��ʼ״̬
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
		// ��ת����
		FRotator NewRotation = TargetMesh->GetRelativeRotation();
		NewRotation.Pitch += FallSpeed * DeltaTime;


		// ��ֹͣ����
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
