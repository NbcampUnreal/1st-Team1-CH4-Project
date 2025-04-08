#include "Character/BricPlayerController.h"
#include "Character/BrickCharacter.h"
#include "EnhancedInputSubsystems.h"

ABricPlayerController::ABricPlayerController()
	:InputMappingContext(nullptr),
	MoveAction(nullptr),
	JumpAction(nullptr),
	LookAction(nullptr),
	StartPlaceBlockAction(nullptr),
	ConfirmPlaceBlockAction(nullptr),
	RotatePreviewBlockAction(nullptr),
	LeftClickAction(nullptr)

{

}

void ABricPlayerController::BeginPlay()
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

