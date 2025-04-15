// Fill out your copyright notice in the Description page of Project Settings.


#include "InGame/CountdownUserWidget.h"
#include "Components/TextBlock.h"

void UCountdownUserWidget::SetCountdownText(const FText& InText)
{
	if (TextBlock_Countdown)
	{
		TextBlock_Countdown->SetText(InText);
	}
}

void UCountdownUserWidget::PlayCountdownAnimation()
{
	if (CountdownScaleAnim)
	{
		PlayAnimation(CountdownScaleAnim);
	}
}

void UCountdownUserWidget::NativeConstruct()
{
	Super::NativeConstruct();
}
