// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EffectBlock/EffectBlockBase.h"
#include "BlueEffectBlock.generated.h"

/**
 *
 */
UCLASS()
class BRICKGAMEPROJECT_API ABlueEffectBlock : public AEffectBlockBase
{
	GENERATED_BODY()

public:
	ABlueEffectBlock();

	float InvincibilityTime;
	float EffectDuration;

	virtual void ApplyEffect(ACharacter* Target) override;
};
