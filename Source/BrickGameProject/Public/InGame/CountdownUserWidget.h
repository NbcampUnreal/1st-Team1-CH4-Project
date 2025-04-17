// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CountdownUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class BRICKGAMEPROJECT_API UCountdownUserWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable)
	void SetCountdownText(const FText& InText);

	/** C++에서 애니메이션 재생 */
	void PlayCountdownAnimation();

protected:
	virtual void NativeConstruct() override;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* TextBlock_Countdown;

	UPROPERTY(meta = (BindWidgetAnim), Transient)
	class UWidgetAnimation* CountdownScaleAnim;
};
