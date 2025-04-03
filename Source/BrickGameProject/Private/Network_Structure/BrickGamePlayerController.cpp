#include "Network_Structure/BrickGamePlayerController.h"
#include "Network_Structure/BrickGamePlayerState.h"
#include "Kismet/GameplayStatics.h"
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


void ABrickGamePlayerController::Server_SetTeam_Implementation(EGameTeam SelectedTeam)
{
	ABrickGamePlayerState* PS = GetBrickGamePlayerState();
	if (PS)
	{
		PS->SetTeam(SelectedTeam);
	}
	
	
}

void ABrickGamePlayerController::Server_SetReady_Implementation(bool bReady)
{
	ABrickGamePlayerState* PS = GetBrickGamePlayerState();
	if (PS)
	{
		PS->SetReady(bReady);
	}
}


ABrickGamePlayerState* ABrickGamePlayerController::GetBrickGamePlayerState() const
{
	return Cast<ABrickGamePlayerState>(PlayerState);
}