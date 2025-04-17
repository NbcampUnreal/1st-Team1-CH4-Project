// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TrapSettingUserWidget.generated.h"

class UButton;
class UTrapButtonWidget;
class ASlipTrap;
class AExplosionTrap;
class AFallTrap;
class ABlockTrap;
/**
 * 
 */
UCLASS()
class BRICKGAMEPROJECT_API UTrapSettingUserWidget : public UUserWidget
{
	GENERATED_BODY()

public:
    virtual void NativeConstruct() override;

protected:
    UPROPERTY(meta = (BindWidget))
    UButton* GoNextCameraButton;

    UPROPERTY(meta = (BindWidget))
    UButton* GoPrevCameraButton;

    UPROPERTY(meta = (BindWidget))
    UTrapButtonWidget* SlipTrapButton;

    UPROPERTY(meta = (BindWidget))
    UTrapButtonWidget* ExplosionTrapButton;

    UPROPERTY(meta = (BindWidget))
    UTrapButtonWidget* FallTrapButton;

    UPROPERTY(meta = (BindWidget))
    UTrapButtonWidget* BlockTrapButton;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Trap")
    TSubclassOf<ASlipTrap> TrapType_Slip;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Trap")
    TSubclassOf<AExplosionTrap> TrapType_Explosion;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Trap")
    TSubclassOf<AFallTrap> TrapType_Fall;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Trap")
    TSubclassOf<ABlockTrap> TrapType_Block;


    UFUNCTION()
    void OnGoNextCameraClicked();

    UFUNCTION()
    void OnGoPrevCameraClicked();
};
