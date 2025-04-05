#include "Network_Structure/BrickGamePlayerController.h"
#include "Network_Structure/BrickGamePlayerState.h"
#include "Network_Structure/BrickLobbyGameMode.h"
#include "Network_Structure/LobbyUserWidget.h"
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
	, FKeyAction(nullptr)
{
	PrimaryActorTick.bCanEverTick = true;
}

void ABrickGamePlayerController::BeginPlay()
{
	Super::BeginPlay();
	const ENetMode NetMode = GetNetMode();
	FString NetModeStr;
	switch (NetMode)
	{
	case NM_Standalone:      NetModeStr = TEXT("Standalone"); break;
	case NM_ListenServer:    NetModeStr = TEXT("ListenServer"); break;
	case NM_DedicatedServer: NetModeStr = TEXT("DedicatedServer"); break;
	case NM_Client:          NetModeStr = TEXT("Client"); break;
	}
	UE_LOG(LogTemp, Warning, TEXT("NetMode: %s"), *NetModeStr);
	UE_LOG(LogTemp, Warning, TEXT("HasAuthority: %s"), HasAuthority() ? TEXT("true") : TEXT("false"));
	UE_LOG(LogTemp, Warning, TEXT("IsLocalController: %s"), IsLocalController() ? TEXT("true") : TEXT("false"));
	FString aRole = HasAuthority() ? TEXT("Server") : TEXT("Client");
	FString aLocal = IsLocalPlayerController() ? TEXT("Local") : TEXT("Remote");

	FString Msg = FString::Printf(TEXT("[DEBUG] %s | %s"), *aRole, *aLocal);

	UE_LOG(LogTemp, Warning, TEXT("%s"), *Msg);

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, Msg);
	}

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

		LobbyWidget = CreateWidget<ULobbyUserWidget>(this, LobbyWidgetClass);
		if (LobbyWidget)
		{
			LobbyWidget->AddToViewport();
			UE_LOG(LogTemp, Warning, TEXT("AddToViewport"));
			SetShowMouseCursor(true);

			FInputModeUIOnly InputMode;
			SetInputMode(InputMode);
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

