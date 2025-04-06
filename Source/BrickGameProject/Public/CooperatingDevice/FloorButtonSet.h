// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FloorButtonSet.generated.h"

class ATwoPlayersFloorButton;

UCLASS()
class BRICKGAMEPROJECT_API AFloorButtonSet : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFloorButtonSet();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Switch Set")
	ATwoPlayersFloorButton* Button1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Switch Set")
	ATwoPlayersFloorButton* Button2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Switch Set")
	AActor* TargetBridge; 


	bool bIsActive;

	void CheckButtonStatus();
};
