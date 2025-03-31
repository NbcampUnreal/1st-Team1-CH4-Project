// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Traps/TrapBase.h"
#include "ExplosionTrap.generated.h"

class URadialForceComponent;
/**
 * 
 */
UCLASS()
class LEGOPROJECT_0_API AExplosionTrap : public ATrapBase
{
	GENERATED_BODY()

public:
	AExplosionTrap();

	UPROPERTY(VisibleAnywhere, Category = "Components")
	URadialForceComponent* RadialForceComp;

	UPROPERTY(EditDefaultsOnly, Category = "Explosion")
	float ExplosionForce = 2000.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Explosion")
	float ExplosionRadius = 500.0f;

	virtual void ActiveTrap(ACharacter* Target) override;
	
};
