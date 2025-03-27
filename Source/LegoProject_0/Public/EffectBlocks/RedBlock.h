// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EffectBlocks/BlockBase.h"
#include "RedBlock.generated.h"

/**
 * 
 */
UCLASS()
class LEGOPROJECT_0_API ARedBlock : public ABlockBase
{
	GENERATED_BODY()
	
public:
	ARedBlock();

	float SpeedBoostValue;
	float ImpulseValue;
	float EffectDuration;

	virtual void ApplyEffect(ACharacter* Target) override;
};
