// Fill out your copyright notice in the Description page of Project Settings.


#include "CooperatingDevice/OnePlayerFloorButton.h"
#include "Kismet/GameplayStatics.h"

void AOnePlayerFloorButton::UpdateButtonState()
{
	Super::UpdateButtonState();

	Door->SetDoorVisible(!bIsPressed);

	if (ButtonToggleSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, ButtonToggleSound, GetActorLocation());
	}
}

