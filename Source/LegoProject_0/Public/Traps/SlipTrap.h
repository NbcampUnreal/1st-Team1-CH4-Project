// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Traps/TrapBase.h"
#include "SlipTrap.generated.h"

class UPhysicalMaterial;
/**
 * 
 */
UCLASS()
class LEGOPROJECT_0_API ASlipTrap : public ATrapBase
{
	GENERATED_BODY()

public:
	ASlipTrap();
protected:
	UPROPERTY(EditDefaultsOnly, Category = "GameDesignSettings")
	float SlipTrapGroundFriction;

	float OriginGroundFriction;
	float OriginBreakingDecelerationWalking;

public:
	virtual void BeginPlay() override;
	virtual void ActiveTrap(ACharacter* Target) override;
	virtual void DeactiveTrap(ACharacter* Target) override;
};
