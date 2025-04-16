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

	UPROPERTY(EditDefaultsOnly, Category = "GameDesignSettings")
	float SpeedBoostValue;
	UPROPERTY(EditDefaultsOnly, Category = "GameDesignSettings")
	float LaunchPower;
	UPROPERTY(EditDefaultsOnly, Category = "GameDesignSettings")
	float EffectDuration;

	virtual void ApplyEffect(ACharacter* Target) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SFX")
	USoundBase* BoostSound;

};
