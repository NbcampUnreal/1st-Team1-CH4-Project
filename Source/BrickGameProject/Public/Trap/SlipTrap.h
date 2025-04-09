// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Trap/TrapBase.h"
#include "SlipTrap.generated.h"

/**
 *
 */
UCLASS()
class BRICKGAMEPROJECT_API ASlipTrap : public ATrapBase
{
	GENERATED_BODY()
	
public:
	ASlipTrap();
protected:
	UPROPERTY(EditDefaultsOnly, Category = "GameDesignSettings")
	float SlipTrapGroundFriction;

	float OriginGroundFriction;
	float OriginBreakingDecelerationWalking;
	float OriginMaxAcceleration;
	float OriginMaxWalkSpeed;

	UPROPERTY(EditAnywhere, Category = "Animation")
	UAnimMontage* FallDownMontage;

	UPROPERTY(EditAnywhere, Category = "Animation")
	UAnimMontage* StandUpMontage;


public:
	virtual void BeginPlay() override;
	virtual void ActiveTrap(ACharacter* Target) override;
	virtual void DeactiveTrap(ACharacter* Target) override;
};
