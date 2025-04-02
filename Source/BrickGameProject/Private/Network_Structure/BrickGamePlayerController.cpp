#include "Network_Structure/BrickGamePlayerController.h"
#include "EnhancedInputSubsystems.h"

ABrickGamePlayerController::ABrickGamePlayerController()
	:InputMappingContext(nullptr),
	MoveAction(nullptr),
	JumpAction(nullptr),
	LookAction(nullptr),
	StartPlaceBlockAction(nullptr),
	ConfirmPlaceBlockAction(nullptr),
	RotatePreviewBlockAction(nullptr)
{
}

void ABrickGamePlayerController::BeginPlay()
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
