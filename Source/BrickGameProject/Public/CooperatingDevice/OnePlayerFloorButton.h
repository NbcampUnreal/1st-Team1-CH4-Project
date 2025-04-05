// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CooperatingDevice/FloorButton.h"
#include "CooperatingDevice/DoorActor.h"
#include "OnePlayerFloorButton.generated.h"

UCLASS()
class BRICKGAMEPROJECT_API AOnePlayerFloorButton : public AFloorButton
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Door")
	ADoorActor* Door;

	virtual void UpdateButtonState() override;
};
