// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HubUserWidget.generated.h"

class UButton;
class UEditableTextBox;
/**
 * 
 */
UCLASS()
class BRICKGAMEPROJECT_API UHubUserWidget : public UUserWidget
{
	GENERATED_BODY()

public:
    virtual void NativeConstruct() override;

protected:
    UFUNCTION()
    void OnCreateSessionClicked();

    UFUNCTION()
    void OnJoinSessionClicked();

    UPROPERTY(meta = (BindWidget))
    UButton* CreateSessionButton;

    UPROPERTY(meta = (BindWidget))
    UButton* JoinSessionButton;

    UPROPERTY(meta = (BindWidget))
    UEditableTextBox* IPAddressBox;
};
