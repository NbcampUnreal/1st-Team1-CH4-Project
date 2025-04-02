// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Trap/TrapBase.h"
#include "BlockTrap.generated.h"

/**
 * 
 */
UCLASS()
class BRICKGAMEPROJECT_API ABlockTrap : public ATrapBase
{
	GENERATED_BODY()
	
public:
	void DisappearBlockTrap();
};
