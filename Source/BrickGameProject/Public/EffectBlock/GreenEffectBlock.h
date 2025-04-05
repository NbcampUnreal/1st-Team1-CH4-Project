// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EffectBlock/EffectBlockBase.h"
#include "GreenEffectBlock.generated.h"

class UArrowComponent;
/**
 *
 */
UCLASS()
class BRICKGAMEPROJECT_API AGreenEffectBlock : public AEffectBlockBase
{
	GENERATED_BODY()

public:
	AGreenEffectBlock();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UArrowComponent* ArrowComponent;
	float JumpBoostValue;

	virtual void ApplyEffect(ACharacter* Target) override;
};
