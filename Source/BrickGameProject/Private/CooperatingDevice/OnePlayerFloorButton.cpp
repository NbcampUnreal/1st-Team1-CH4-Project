// Fill out your copyright notice in the Description page of Project Settings.


#include "CooperatingDevice/OnePlayerFloorButton.h"

void AOnePlayerFloorButton::UpdateButtonState()
{
	Super::UpdateButtonState();

	Door->SetDoorVisible(!bIsPressed);
}
