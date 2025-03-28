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
	// ���� ��Ƽ���� (�̲����� ǥ��)
	UPROPERTY(EditAnywhere, Category = "Slippery")
	UPhysicalMaterial* SlipperyMaterial;
public:
	virtual void BeginPlay() override;
	virtual void OperateTrap(ACharacter* Target) override;
};
