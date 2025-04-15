// Fill out your copyright notice in the Description page of Project Settings.


#include "InGame/TrapSettingUserWidget.h"
#include "Components/Button.h"
#include "Network_Structure/BrickGamePlayerController.h"

void UTrapSettingUserWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (GoNextCameraButton) GoNextCameraButton->OnClicked.AddDynamic(this, &UTrapSettingUserWidget::OnGoNextCameraClicked);
	if (GoPrevCameraButton) GoPrevCameraButton->OnClicked.AddDynamic(this, &UTrapSettingUserWidget::OnGoPrevCameraClicked);
}

void UTrapSettingUserWidget::OnGoNextCameraClicked()
{
	if (ABrickGamePlayerController* BPC = Cast<ABrickGamePlayerController>(GetOwningPlayer()))
	{
		BPC->PlayPlacementCameraSequence(true);
	}
}

void UTrapSettingUserWidget::OnGoPrevCameraClicked()
{
	if (ABrickGamePlayerController* BPC = Cast<ABrickGamePlayerController>(GetOwningPlayer()))
	{
		BPC->PlayPlacementCameraSequence(false);
	}
}
