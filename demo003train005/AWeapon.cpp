// Fill out your copyright notice in the Description page of Project Settings.


#include "AWeapon.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"

// Sets default values
AAWeapon::AAWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
//	PrimaryActorTick.bCanEverTick = true;
	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	RootComponent = Mesh;
	CurrentAmmo = MaxAmmo;

}
void AAWeapon::Fire()
{
    FVector Start = Mesh->GetSocketLocation("Muzzle");
    FVector End = Start + Mesh->GetForwardVector() * 10000;

    FHitResult Hit;
    FCollisionQueryParams Params;
    Params.AddIgnoredActor(this);

    if (GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, Params))
    {
        DrawDebugLine(GetWorld(), Start, Hit.Location, FColor::Red, false, 1.f);
    }
}


// Called when the game starts or when spawned
void AAWeapon::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AAWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

