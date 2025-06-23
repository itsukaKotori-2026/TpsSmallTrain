//�ڡ���Ŀ���á��ġ�������ҳ������д���İ�Ȩ������

//���� TargetActor.h ͷ�ļ���ȷ�����.cpp �ļ�֪����ͳ�Ա������������
#include "ATargetActor.h"
#include "UObject/ConstructorHelpers.h"

// ���캯��
AATargetActor::AATargetActor()
{
 	// ��ʾ��� Actor ������ Tick��ÿһ֡������� Tick ������
	PrimaryActorTick.bCanEverTick = true;

	/*����һ�� ��̬��������� ��Ϊ���ӵ� 3D ģ�͡�
	��������Ϊ ������������������� Actor ��λ�á���ת�����š�*/
	TargetMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TargetMesh"));
	RootComponent = TargetMesh;

	// �Զ������������� Cylinder ģ�ͣ�������سɹ������丳�� TargetMesh��
	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset(TEXT("/Engine/BasicShapes/Cylinder.Cylinder"));
	if (MeshAsset.Succeeded())
	{
		TargetMesh->SetStaticMesh(MeshAsset.Object);
	}
	// �Զ������������ò��ʣ�������سɹ������丳�� TargetMesh��
	static ConstructorHelpers::FObjectFinder<UMaterial> MaterialAsset(TEXT("/Engine/BasicShapes/BasicShapeMaterial"));
	if (MaterialAsset.Succeeded())
	{
		TargetMesh->SetMaterial(0, MaterialAsset.Object);
	}
	// �������ţ�������ӵ�Բ��
	TargetMesh->SetWorldScale3D(FVector(0.5f, 0.5f, 0.05f));
	// ������ײ
	//������ѯ��������ײ�����ܼ������ / �ص���Ҳ����������
	//���ö�������Ϊ WorldDynamic����̬���壬��������ƶ������壩
	//��������ײͨ���赲���ӵ�����ɫ�����߶��ᱻ��ס��
	TargetMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	TargetMesh->SetCollisionObjectType(ECC_WorldDynamic);
	TargetMesh->SetCollisionResponseToAllChannels(ECR_Block);

	// ȷ������û�и��� SetActorRotation
	TargetMesh->SetSimulatePhysics(false);
	//  ��ʼ����
	//bIsFalling = false����ʼʱ���Ӳ����¡�
	//FallSpeed = 100.0f�����µ���ת�ٶȣ���λ�Ƕ� / �롣
	bIsFalling = false;
	FallSpeed = 100.0f;

}

//  ��Ϸ��ʼʱ�����ã�����û�ж����߼���ֻ�ǵ��û��� BeginPlay()��
void AATargetActor::BeginPlay()
{
	Super::BeginPlay();
	UE_LOG(LogTemp, Warning, TEXT("BeginPlay TargetMesh forward: %s"), *TargetMesh->GetForwardVector().ToString());
	UE_LOG(LogTemp, Warning, TEXT("BeginPlay TargetMesh up: %s"), *TargetMesh->GetUpVector().ToString());
	
}

// ÿ֡���°��ӵ�״̬��ʵ�ֱ����к�ĵ��¶�����
void AATargetActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (bIsFalling) {//��� bIsFalling �Ƿ�Ϊ true���Ƿ񱻻��У���

		// ��ȡ��ǰ��ת�Ƕ�
		FRotator NewRotation = TargetMesh->GetRelativeRotation();
		NewRotation.Roll += FallSpeed * DeltaTime; // ÿ֡����һ������תֵ


		// �� Pitch �ﵽ�򳬹� 90 �ȣ�������ȫ���£�ʱ��ֹͣ��ת��bIsFalling = false����
		if (NewRotation.Roll >= 0.0f)
		{
			NewRotation.Roll = 0.0f; // ��ֹ���� 90��
			bIsFalling = false;  // ֹͣ��ת

		}
		// ������ת
		TargetMesh->SetRelativeRotation(NewRotation);
		UE_LOG(LogTemp, Warning, TEXT("Target is falling... Roll: %.2f"), NewRotation.Roll);

	}
}
//�����ӱ��ӵ�����ʱ���ã��������¶��������ⲿ�����ɫ�� OnFire() ���߼�⣩���á�
void AATargetActor::OnHitByBullet()
{
	if (!bIsFalling) //��������δ����ʱ��!bIsFalling���Żᴥ����
	{
		bIsFalling = true; // ��������
		UE_LOG(LogTemp, Warning, TEXT("Target starts falling..."));
	}
}

