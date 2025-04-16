#include "Network_Structure/BrickGamePlayerController.h"
#include "Network_Structure/BrickGamePlayerState.h"
#include "Network_Structure/BrickLobbyGameMode.h"
#include "Network_Structure/BrickInGameMode.h"
#include "Network_Structure/LobbyUserWidget.h"
#include "Network_Structure/HubUserWidget.h"
#include "Network_Structure/BrickInGameState.h"
#include "Network_Structure/BrickGameInstance.h"
#include "InGame/InGameHUD.h"
#include "Network_Structure/EGameTeam.h"
#include "Kismet/GameplayStatics.h"
#include "EngineUtils.h"
#include "Camera/CameraActor.h"
#include "EnhancedInputSubsystems.h"
#include "InGame/TrapSettingUserWidget.h"
#include "Trap/TrapBase.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "InGame/CountdownUserWidget.h"
#include "Character/BrickCharacter.h"
#include "InGame/LoadingUserWidget.h"
#include "InGame/MapViewUserWidget.h"


ABrickGamePlayerController::ABrickGamePlayerController()
	: LobbyWidget(nullptr)
	, HubWidget(nullptr)
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
			if (MapName.Contains("TestLevel"))
			{
				UE_LOG(LogTemp, Warning, TEXT("LoadingUIInit"));
				InitLoadingUI();
			}
			else if (MapName.Contains("LobbyLevel"))
			{
				InitLobbyUI();
			}
			else if (MapName.Contains("HubLevel"))
			{
				InitHubUI();
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
		if (MapName.Contains("TestLevel"))
		{
			UE_LOG(LogTemp, Warning, TEXT("LoadingUIInit"));
			InitLoadingUI();
		}
		else if (MapName.Contains("LobbyLevel"))
		{
			InitLobbyUI();
		}
		else if (MapName.Contains("HubLevel"))
		{
			InitHubUI();
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
		LobbyWidget->ActivateStartButton(bEnable);
	}
}



ABrickGamePlayerState* ABrickGamePlayerController::GetBrickGamePlayerState() const
{
	return Cast<ABrickGamePlayerState>(PlayerState);
}

// Init UI
void ABrickGamePlayerController::InitLobbyUI()
{
	if (InGameHUDWidget)
	{
		InGameHUDWidget->RemoveFromParent();
		InGameHUDWidget = nullptr;
	}

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
	if (CountdownWidget)
	{
		CountdownWidget->RemoveFromParent();
		CountdownWidget = nullptr;
	}

	InGameHUDWidget = CreateWidget<UInGameHUD>(this, InGameHUDClass);
	if (InGameHUDWidget)
	{
		InGameHUDWidget->AddToViewport();

		FInputModeGameOnly InputMode;
		SetInputMode(InputMode);
		SetShowMouseCursor(false);
	}
}

void ABrickGamePlayerController::InitHubUI()
{
	if (LobbyWidget)
	{
		LobbyWidget->RemoveFromParent();
		LobbyWidget = nullptr;
	}

	HubWidget = CreateWidget<UHubUserWidget>(this, HubWidgetClass);
	if (HubWidget)
	{
		HubWidget->AddToViewport();
		SetShowMouseCursor(true);

		FInputModeUIOnly InputMode;
		SetInputMode(InputMode);
	}
}

void ABrickGamePlayerController::InitTrapSettingUI()
{
	if (MapViewWidget)
	{
		MapViewWidget->RemoveFromParent();
		MapViewWidget = nullptr;
	}

	TrapSettingWidget = CreateWidget<UTrapSettingUserWidget>(this, TrapSettingWidgetClass);
	if (TrapSettingWidget)
	{
		TrapSettingWidget->AddToViewport();

		FInputModeGameAndUI InputMode;
		InputMode.SetWidgetToFocus(TrapSettingWidget->TakeWidget());
		InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
		InputMode.SetHideCursorDuringCapture(false);
		SetInputMode(InputMode);

		bShowMouseCursor = true;
	}
}

void ABrickGamePlayerController::InitCountdownUI()
{
	if (TrapSettingWidget)
	{
		TrapSettingWidget->RemoveFromParent();
		TrapSettingWidget = nullptr;
	}

	CountdownWidget = CreateWidget<UCountdownUserWidget>(this, CountdownWidgetClass);
	if (CountdownWidget)
	{
		CountdownWidget->AddToViewport();

		FInputModeGameOnly InputMode;
		SetInputMode(InputMode);
		SetShowMouseCursor(false);
		FSlateApplication::Get().CancelDragDrop();
	}
}

void ABrickGamePlayerController::InitLoadingUI()
{
	if (LobbyWidget)
	{
		LobbyWidget->RemoveFromParent();
		LobbyWidget = nullptr;
	}

	LoadingWidget = CreateWidget<ULoadingUserWidget>(this, LoadingWidgetClass);
	if (LoadingWidget)
	{
		LoadingWidget->AddToViewport();
		FInputModeUIOnly InputMode;
		SetInputMode(InputMode);
		SetShowMouseCursor(false);
	}
}

void ABrickGamePlayerController::InitMapViewUI()
{
	if (LoadingWidget)
	{
		LoadingWidget->RemoveFromParent();
		LoadingWidget = nullptr;
	}

	MapViewWidget = CreateWidget<UMapViewUserWidget>(this, MapViewWidgetClass);
	if (MapViewWidget)
	{
		MapViewWidget->AddToViewport();
		FInputModeUIOnly InputMode;
		SetInputMode(InputMode);
		SetShowMouseCursor(false);
	}
}

// Camera Operation
void ABrickGamePlayerController::PlayIntroCameraSequence()
{
	if (!IsLocalPlayerController()) return;

	if (Cameras.Num() == 0 && CurrentCameraIndex == 0)
	{
		FindCamerasByTag("IntroViewCamera"); // 태그로 카메라 모으기
		CurrentCameraIndex = 0;
	}

	// 카메라 전환
	if (CurrentCameraIndex < Cameras.Num())
	{
		UE_LOG(LogTemp, Warning, TEXT("CameraChange"));
		SetViewTarget(Cast<AActor>(Cameras[CurrentCameraIndex]));
		CurrentCameraIndex++;
		SetViewTargetWithBlend(Cast<AActor>(Cameras[CurrentCameraIndex]), 5.0f);
	}

}

void ABrickGamePlayerController::PlayPlacementCameraSequence(bool bNext)
{
	UE_LOG(LogTemp, Warning, TEXT("PlacementCameraSequence"));
	if (Cameras.Num() == 0 && CurrentCameraIndex == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("PlacementCameraSequence Reset"));
		ABrickGamePlayerState* PS = GetBrickGamePlayerState();
		if (PS)
		{
			FName CameraTag;
			if (PS->GetTeam() == EGameTeam::Red)
			{
				CameraTag = FName(*FString::Printf(TEXT("PlacementCamera_Red")));
			}
			else if (PS->GetTeam() == EGameTeam::Blue)
			{
				CameraTag = FName(*FString::Printf(TEXT("PlacementCamera_Blue")));
			}
			
			FindCamerasByTag(CameraTag); // 태그로 카메라 모으기
			CurrentCameraIndex = 0;
			SetViewTarget(Cast<AActor>(Cameras[CurrentCameraIndex]));
			return;
		}	
	}

	if (Cameras.Num() == 0) return;

	if (bNext)
	{
		// Go Next Camera
		CurrentCameraIndex = (CurrentCameraIndex + 1) % Cameras.Num();
	}
	else
	{
		// Go Prev Camera
		CurrentCameraIndex = (CurrentCameraIndex - 1 + Cameras.Num()) % Cameras.Num();
	}

	SetViewTarget(Cast<AActor>(Cameras[CurrentCameraIndex]));
}

void ABrickGamePlayerController::ReturnToPawnCamera()
{
	APawn* ControlledPawn = GetPawn();
	if (ControlledPawn)
	{
		SetViewTarget(ControlledPawn);
		UE_LOG(LogTemp, Warning, TEXT("Return to viewtarget : %s"), *ControlledPawn->GetName());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("No Pawn"));
	}
}

void ABrickGamePlayerController::FindCamerasByTag(FName TagName)
{
	Cameras.Empty();

	FName CameraTag = TagName;

	for (TActorIterator<ACameraActor> It(GetWorld()); It; ++It)
	{
		ACameraActor* Cam = *It;
		if (Cam && Cam->ActorHasTag(CameraTag))
		{
			Cameras.Add(Cam);
		}
	}

	// Sort Camera(by name)
	Cameras.Sort([](const ACameraActor& A, const ACameraActor& B)
	{
		return A.GetName() < B.GetName();
	});
	UE_LOG(LogTemp, Warning, TEXT("FindIntroCameras : %d"), Cameras.Num());
}

void ABrickGamePlayerController::ResetCameras()
{
	Cameras.Empty();
	CurrentCameraIndex = 0;
}

void ABrickGamePlayerController::UpdateCountdownUI(int32 InCountdown)
{
	if (CountdownWidget)
	{
		if (InCountdown > 0)
		{

			CountdownWidget->SetVisibility(ESlateVisibility::Visible);
			CountdownWidget->SetCountdownText(FText::FromString(FString::FromInt(InCountdown)));
			CountdownWidget->PlayCountdownAnimation();

		}
		else
		{
			CountdownWidget->SetVisibility(ESlateVisibility::Hidden);
		}
	}
}

void ABrickGamePlayerController::HandleTrapDrop(FVector2D ScreenPosition, TSubclassOf<ATrapBase> TrapClass)
{
	FVector WorldOrigin;
	FVector WorldDirection;

	// 1. 마우스 위치 기준으로 월드 방향 계산
	if (DeprojectScreenPositionToWorld(ScreenPosition.X, ScreenPosition.Y, WorldOrigin, WorldDirection))
	{
		// 2. 카메라 위치 기준으로 시작점 설정
		FVector CameraLocation;
		FRotator CameraRotation;
		GetPlayerViewPoint(CameraLocation, CameraRotation);

		FVector Start = CameraLocation;
		FVector End = Start + WorldDirection * 10000.0f;

		FHitResult Hit;
		//DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, 5.0f, 0, 2.0f);

		if (GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility))
		{
			UE_LOG(LogTemp, Warning, TEXT("LineTrace Success: %s"), *Hit.Location.ToString());

			if (HasAuthority())
			{
				FActorSpawnParameters SpawnParams;
				SpawnParams.Owner = this;
				SpawnParams.Instigator = GetPawn();

				GetWorld()->SpawnActor<ATrapBase>(TrapClass, Hit.Location, FRotator::ZeroRotator, SpawnParams);
			}
			else
			{
				Server_SpawnTrap(Hit.Location, TrapClass);
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("LineTrace Failed: Start=%s End=%s"), *Start.ToString(), *End.ToString());
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Deproject Failed: ScreenPos=(%f, %f)"), ScreenPosition.X, ScreenPosition.Y);
	}
}

void ABrickGamePlayerController::Server_SpawnTrap_Implementation(FVector Location, TSubclassOf<ATrapBase> TrapClass)
{
	UE_LOG(LogTemp, Warning, TEXT("Trap_install_by client"));
	FVector TrapSpawnLocation = Location;
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.Instigator = GetPawn();

	GetWorld()->SpawnActor<ATrapBase>(TrapClass, TrapSpawnLocation, FRotator::ZeroRotator, SpawnParams);
}