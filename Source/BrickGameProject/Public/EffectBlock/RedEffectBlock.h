// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EffectBlock/EffectBlockBase.h"
#include "RedEffectBlock.generated.h"

/**
 *
 */
UCLASS()
class BRICKGAMEPROJECT_API ARedEffectBlock : public AEffectBlockBase
{
	GENERATED_BODY()

public:
	ARedEffectBlock();

	float SpeedBoostValue;
	float ImpulseValue;
	float EffectDuration;

	virtual void ApplyEffect(ACharacter* Target) override;
};
