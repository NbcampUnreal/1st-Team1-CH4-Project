// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CooperatingDevice/FloorButton.h"
#include "TwoPlayersFloorButton.generated.h"

/**
 * 
 */
UCLASS()
class BRICKGAMEPROJECT_API ATwoPlayersFloorButton : public AFloorButton
{
	GENERATED_BODY()

public:
	// ��ư ��Ʈ
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Switch")
	AFloorButtonSet* ButtonSet;

	UPROPERTY(EditDefaultsOnly)
	UMaterialInterface* CompletedMaterial;

	virtual void UpdateButtonState() override;
	void LockButton();
	
};
