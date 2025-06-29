// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TargetActor.generated.h"

UCLASS()
class DEMO002REVIEW001_API ATargetActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATargetActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	// ����ʱ����
	//myadd
	void OnHitByBullet();
//myadd
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

