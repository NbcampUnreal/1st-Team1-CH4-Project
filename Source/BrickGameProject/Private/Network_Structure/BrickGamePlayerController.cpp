#include "Network_Structure/BrickGamePlayerController.h"
#include "Network_Structure/BrickGamePlayerState.h"
#include "Network_Structure/BrickLobbyGameMode.h"
#include "Network_Structure/BrickInGameMode.h"
#include "Network_Structure/LobbyUserWidget.h"
#include "Network_Structure/HubUserWidget.h"
#include "Network_Structure/BrickInGameState.h"
#include "Network_Structure/BrickGameInstance.h"
#include "Network_Structure/EGameTeam.h"
#include "Kismet/GameplayStatics.h"
#include "EngineUtils.h"
#include "Camera/CameraActor.h"
#include "EnhancedInputSubsystems.h"
#include "InGame/TrapSettingUserWidget.h"
#include "Trap/TrapBase.h"


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
			if (MapName.Contains("InGameLevel"))
			{
				InitInGameUI();
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
		if (MapName.Contains("InGameLevel"))
		{
			InitInGameUI();
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
		LobbyWidget->ActivateStartButton();
	}
}


ABrickGamePlayerState* ABrickGamePlayerController::GetBrickGamePlayerState() const
{
	return Cast<ABrickGamePlayerState>(PlayerState);
}

// Init UI
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

	if (TrapSettingWidget)
	{
		TrapSettingWidget->RemoveFromParent();
		TrapSettingWidget = nullptr;
	}

	FInputModeGameOnly InputMode;
	SetInputMode(InputMode);
	SetShowMouseCursor(false);
	/*InGameWidget = CreateWidget<UInGameUserWidget>(this, InGameWidgetClass);
	if (InGameWidget)
	{
		InGameWidget->AddToViewport();
		SetShowMouseCursor(false);
		FInputModeGameOnly InputMode;
		SetInputMode(InputMode);
	}*/
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
	if (LobbyWidget)
	{
		LobbyWidget->RemoveFromParent();
		LobbyWidget = nullptr;
	}

	TrapSettingWidget = CreateWidget<UTrapSettingUserWidget>(this, TrapSettingWidgetClass);
	if (TrapSettingWidget)
	{
		TrapSettingWidget->AddToViewport();
		FInputModeGameAndUI InputMode;
		InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
		InputMode.SetHideCursorDuringCapture(false);
		SetInputMode(InputMode);
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
		SetViewTargetWithBlend(Cast<AActor>(Cameras[CurrentCameraIndex]), 10.0f);
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

void ABrickGamePlayerController::HandleTrapDrop(FVector2D ScreenPosition, TSubclassOf<ATrapBase> TrapClass)
{
	
	/*FVector WorldLocation;
	FVector WorldDirection;*/

	FVector2D ViewportSize;
	GEngine->GameViewport->GetViewportSize(ViewportSize);
	FVector2D ViewportCenter = ViewportSize / 2;

	FVector WorldLocation;
	FVector WorldDirection;
	
	// 1. 스크린 좌표를 3D 월드로 변환
	//if (DeprojectScreenPositionToWorld(ScreenPosition.X, ScreenPosition.Y, WorldLocation, WorldDirection))
	if(DeprojectScreenPositionToWorld(ViewportCenter.X, ViewportCenter.Y, WorldLocation, WorldDirection))
	{
		// 2. 라인 트레이스를 아래 방향으로 쏴서 지형 히트 확인
		FHitResult Hit;
		FVector Start = WorldLocation;
		FVector End = Start + WorldDirection * 10000.0f; // 너무 멀 필요 없음, 10,000도 충분


		DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, 5.0f, 0, 2.0f);
		if (GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility))
		{
			UE_LOG(LogTemp, Warning, TEXT("HandleTrapDrop_2"));
			if (HasAuthority()) // 서버에서만 스폰
			{
				FVector TrapSpawnLocation = Hit.Location;
				FActorSpawnParameters SpawnParams;
				SpawnParams.Owner = this;
				SpawnParams.Instigator = GetPawn();
				UE_LOG(LogTemp, Warning, TEXT("Trap_install_by server"));
				// 예: 드래그 시 Payload에 담긴 클래스를 사용할 수 있다면 거기서 사용
				GetWorld()->SpawnActor<ATrapBase>(TrapClass, TrapSpawnLocation, FRotator::ZeroRotator, SpawnParams);
			}
			else
			{
				// 클라이언트인 경우, 서버 RPC 요청
				Server_SpawnTrap(Hit.Location, TrapClass);
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("LineTrace Failed: Start %s End %s"), *Start.ToString(), *End.ToString());
		}
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

