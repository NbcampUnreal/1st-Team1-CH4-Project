#include "Network_Structure/BrickGamePlayerController.h"
#include "Network_Structure/BrickGamePlayerState.h"
#include "Network_Structure/BrickLobbyGameMode.h"
#include "Network_Structure/BrickInGameMode.h"
#include "Network_Structure/LobbyUserWidget.h"
#include "Network_Structure/BrickGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "EnhancedInputSubsystems.h"


ABrickGamePlayerController::ABrickGamePlayerController()
	: LobbyWidget(nullptr)
	, InputMappingContext(nullptr)
	, MoveAction(nullptr)
	, JumpAction(nullptr)
	, LookAction(nullptr)
	, StartPlaceBlockAction(nullptr)
	, ConfirmPlaceBlockAction(nullptr)
	, RotatePreviewBlockAction(nullptr)
	, Block1Action(nullptr)
	, Block2Action(nullptr)
	, Block3Action(nullptr)
	, DeleteBlockAction(nullptr)
	, HoldFAction(nullptr)
	, LeftClickAction(nullptr)
{
	PrimaryActorTick.bCanEverTick = true;
}

void ABrickGamePlayerController::BeginPlay()
{
	Super::BeginPlay();


	if (HasAuthority())
	{
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
		if (IsLocalPlayerController())
		{
			FString MapName = GetWorld()->GetMapName();
			if (MapName.Contains("InGameLevel"))
			{
				InitInGameUI();
			}
			else if (MapName.Contains("LobbyLevel"))
			{
				InitLobbyUI();
			}
		}
	}


}

void ABrickGamePlayerController::PostNetInit()
{
	Super::PostNetInit();


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
	if (IsLocalPlayerController())
	{
		FString MapName = GetWorld()->GetMapName(); 
		if (MapName.Contains("InGameLevel"))
		{
			InitInGameUI();
		}
		else if (MapName.Contains("LobbyLevel"))
		{
			InitLobbyUI();
		}
	}
}

void ABrickGamePlayerController::Server_SetPlayerID_Implementation(int32 PlayerID)
{
	ABrickGamePlayerState* PS = GetBrickGamePlayerState();
	if (PS)
	{
		PS->SetBrickPlayerID(PlayerID);
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


void ABrickGamePlayerController::Server_StartGame_Implementation()
{
	ABrickLobbyGameMode* GM = GetWorld()->GetAuthGameMode<ABrickLobbyGameMode>();
	if (GM)
	{
		GM->StartGame();
	}
}


void ABrickGamePlayerController::Client_EnableStartButton_Implementation(bool bEnable)
{

	if (LobbyWidget)
	{
		LobbyWidget->ActivateStartButton();
	}
}


ABrickGamePlayerState* ABrickGamePlayerController::GetBrickGamePlayerState() const
{
	return Cast<ABrickGamePlayerState>(PlayerState);
}

void ABrickGamePlayerController::InitLobbyUI()
{
	/*if (InGameWidget)
	{
		InGameWidget->RemoveFromParent();
		InGameWidget = nullptr;
	}*/

	LobbyWidget = CreateWidget<ULobbyUserWidget>(this, LobbyWidgetClass);
	if (LobbyWidget)
	{
		LobbyWidget->AddToViewport();
		SetShowMouseCursor(true);

		FInputModeUIOnly InputMode;
		SetInputMode(InputMode);
	}
}

void ABrickGamePlayerController::InitInGameUI()
{
	if (LobbyWidget)
	{
		LobbyWidget->RemoveFromParent();
		LobbyWidget = nullptr;
	}

	/*InGameWidget = CreateWidget<UInGameUserWidget>(this, InGameWidgetClass);
	if (InGameWidget)
	{
		InGameWidget->AddToViewport();
		SetShowMouseCursor(false);
		FInputModeGameOnly InputMode;
		SetInputMode(InputMode);
	}*/
}

