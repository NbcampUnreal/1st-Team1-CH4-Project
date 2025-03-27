// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/LegoPlayerController.h"
#include "EnhancedInputSubsystems.h"

ALegoPlayerController::ALegoPlayerController()
	:InputMappingContext(nullptr),
	MoveAction(nullptr),
	JumpAction(nullptr),
	LookAction(nullptr),
	StartPlaceBlockAction(nullptr),
	ConfirmPlaceBlockAction(nullptr),
	RotatePreviewBlockAction(nullptr)
{
}

void ALegoPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (ULocalPlayer* LocalPlayer = GetLocalPlayer())
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
		{
			if (InputMappingContext)
			{
				Subsystem->AddMappingContext(InputMappingContext, 0);
			}
		}
	}
}
