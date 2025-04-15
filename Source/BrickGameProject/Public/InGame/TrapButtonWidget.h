// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TrapButtonWidget.generated.h"

class ATrapBase;
class UTrapSetPointUserWidget;
/**
 * 
 */
UCLASS()
class BRICKGAMEPROJECT_API UTrapButtonWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Trap")
	TSubclassOf<ATrapBase> TrapClassToSpawn;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Icon")
	TSubclassOf<UTrapSetPointUserWidget> TrapSetIcon;

	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) override;
	
};
