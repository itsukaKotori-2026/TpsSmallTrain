
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TargetActor.generated.h"

UCLASS()
class DEMO003REVIEW003_API ATargetActor : public AActor
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
	void OnHitByBullet();
private:

	UPROPERTY(VisibleAnywhere)

	//
	UStaticMeshComponent* TargetMesh;
	//
	bool bIsFalling;
	//
	FVector FallDirection;
	//
	float FallSpeed;
};
