// �ڡ���Ŀ���á��ġ�������ҳ������д���İ�Ȩ������

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ATargetActor.generated.h"

UCLASS()
class FIRSTTPSDEMO002_API AATargetActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Ϊ�˲����ߵ���������Ĭ��ֵ
	AATargetActor();

protected:
	// ����Ϸ��ʼ������ʱ����
	virtual void BeginPlay() override;

public:	
	// ����ÿһ֡
	virtual void Tick(float DeltaTime) override;
	// ����ʱ����
	void OnHitByBullet();
private:
	//UPROPERTY������������������뷴��ϵͳ�����л��ȡ�
	//VisibleAnywhere����ʾ�� �༭���У���ͼ��������壩�ɼ��������ɱ༭��
	UPROPERTY(VisibleAnywhere)

	//����һ��ָ�� ��̬��������� ��ָ�롣 ������������ӵ���ۣ�3D ģ�ͣ���
	UStaticMeshComponent* TargetMesh;
	//����һ������ֵ�� true ��ʾ�������ڵ��� false ��ʾ���Ӿ�ֹ
	bool bIsFalling;
	//����һ����ά��������ʾ ���ӵ��µķ��� ���磺FVector(1, 0, 0) �ͱ�ʾ�� X ���������¡�
	FVector FallDirection;
	//һ������������ʾ�� ���ӵ���ʱ���ٶȣ���λͨ���ǵ�λ / �룩��
	float FallSpeed;
};
