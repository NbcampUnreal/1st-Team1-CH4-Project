// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EffectBlocks/BlockBase.h"
#include "GreenBlock.generated.h"

class UArrowComponent;

UCLASS()
class BRICKGAMEPROJECT_API AGreenBlock : public ABlockBase
{
	GENERATED_BODY()
	
public:
	AGreenBlock();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UArrowComponent* ArrowComponent;
	float JumpBoostValue;

	virtual void ApplyEffect(ACharacter* Target) override;
};
