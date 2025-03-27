// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EffectBlocks/BlockBase.h"
#include "BlueBlock.generated.h"

/**
 * 
 */
UCLASS()
class LEGOPROJECT_0_API ABlueBlock : public ABlockBase
{
	GENERATED_BODY()
public:
	ABlueBlock();

	float InvincibilityTime;
	float EffectDuration;

	virtual void ApplyEffect(ACharacter* Target) override;
};
