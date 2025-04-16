// Fill out your copyright notice in the Description page of Project Settings.


#include "CooperatingDevice/TwoPlayersFloorButton.h"
#include "Kismet/GameplayStatics.h"
#include "CooperatingDevice/FloorButtonSet.h"

void ATwoPlayersFloorButton::UpdateButtonState()
{
	Super::UpdateButtonState();

	if (ButtonSet)
	{
		ButtonSet->CheckButtonStatus();
	}
}

void ATwoPlayersFloorButton::LockButton()
{
	bLocked = true;

	StaticMeshButtonComp->SetRelativeLocation(PressedButtonLocation);
	SetButtonMaterial(CompletedMaterial);

	if (ButtonPressSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, ButtonPressSound, GetActorLocation());
	}
}
