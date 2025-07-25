// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "AWeapon.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MeleeWeapon.generated.h"

UCLASS()
class DEMO008_API AMeleeWeapon : public AAWeapon
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMeleeWeapon();
};
