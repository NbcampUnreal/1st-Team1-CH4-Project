 #pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "EGameTeam.h"
#include "BrickGamePlayerController.generated.h"

class ABrickGamePlayerState;
class UInputMappingContext;
class UInputAction;
class ULobbyUserWidget;
class UInGameHUD;
class UHubUserWidget;
class UTrapSettingUserWidget;
class UCountdownUserWidget;
class ATrapBase;
class ULoadingUserWidget;
class UMapViewUserWidget;

UCLASS()
class BRICKGAMEPROJECT_API ABrickGamePlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ABrickGamePlayerController();

	virtual void BeginPlay() override;
	virtual void PostNetInit() override;


	//RPC

	//Server
	UFUNCTION(Server, Reliable)
	void Server_SetPlayerID(int32 PlayerID);
	UFUNCTION(Server, Reliable)
	void Server_SetTeam(EGameTeam Team);
	UFUNCTION(Server, Reliable)
	void Server_SetReady(bool bReady);
	UFUNCTION(Server, Reliable)
	void Server_StartGame();

	//Client
	UFUNCTION(Client, Reliable)
	void Client_EnableStartButton(bool bEnable);

	ABrickGamePlayerState* GetBrickGamePlayerState() const;

	//Lobby UI
	UPROPERTY(EditDefaultsOnly, Category = "UI|Lobby")
	TSubclassOf<ULobbyUserWidget> LobbyWidgetClass;

	UPROPERTY()
	ULobbyUserWidget* LobbyWidget;

	// Hub UI
	UPROPERTY(EditDefaultsOnly, Category = "UI|Hub")
	TSubclassOf<UHubUserWidget> HubWidgetClass;

	UPROPERTY()
	UHubUserWidget* HubWidget;

	// TrapSetting UI
	UPROPERTY(EditDefaultsOnly, Category = "UI|InGame")
	TSubclassOf<UTrapSettingUserWidget> TrapSettingWidgetClass;

	UPROPERTY()
	UTrapSettingUserWidget* TrapSettingWidget;

	TArray<ACameraActor*> Cameras;
	int32 CurrentCameraIndex = 0;
	FTimerHandle CameraSwitchTimerHandle;

	// Countdown UI
	UPROPERTY(EditDefaultsOnly, Category="UI|InGame")
	TSubclassOf<UCountdownUserWidget> CountdownWidgetClass;

	UPROPERTY()
	UCountdownUserWidget* CountdownWidget;

	//InGame UI
	UPROPERTY(EditDefaultsOnly, Category = "UI|InGame")
	TSubclassOf<UInGameHUD> InGameHUDClass;

	UPROPERTY()
	UInGameHUD* InGameHUDWidget;

	// Loading UI
	UPROPERTY(EditDefaultsOnly, Category = "UI|InGame")
	TSubclassOf<ULoadingUserWidget> LoadingWidgetClass;

	UPROPERTY()
	ULoadingUserWidget* LoadingWidget;

	// MapView UI
	UPROPERTY(EditDefaultsOnly, Category = "UI|InGame")
	TSubclassOf<UMapViewUserWidget> MapViewWidgetClass;

	UPROPERTY()
	UMapViewUserWidget* MapViewWidget;

	void InitLobbyUI();
	void InitInGameUI();
	void InitHubUI();
	void InitTrapSettingUI();
	void InitCountdownUI();
	void InitLoadingUI();
	void InitMapViewUI();

	// Phase
	void PlayIntroCameraSequence();
	void PlayPlacementCameraSequence(bool bNext);
	void ReturnToPawnCamera();
	void FindCamerasByTag(FName TagName);
	void ResetCameras();
	void UpdateCountdownUI(int32 InCountdown);

	UFUNCTION(BlueprintCallable)
	void HandleTrapDrop(FVector2D ScreenPosition, TSubclassOf<ATrapBase> TrapClass);

	UFUNCTION(Server, Reliable)
	void Server_SpawnTrap(FVector Location, TSubclassOf<ATrapBase> TrapClass);

protected:
	//IMC
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputMappingContext* InputMappingContext;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* MoveAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* JumpAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* LookAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* StartPlaceBlockAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* ConfirmPlaceBlockAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* Block1Action;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* Block2Action;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* Block3Action;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* RotatePreviewBlockAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* DeleteBlockAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	UInputAction* HoldFAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	class UInputAction* LeftClickAction;


public:
	//getter, setter
	UInputMappingContext* GetInputMappingContext() const { return InputMappingContext; }
	void SetInputMappingContext(UInputMappingContext* NewContext) { InputMappingContext = NewContext; }

	UInputAction* GetMoveAction() const { return MoveAction; }
	void SetMoveAction(UInputAction* NewAction) { MoveAction = NewAction; }

	UInputAction* GetJumpAction() const { return JumpAction; }
	void SetJumpAction(UInputAction* NewAction) { JumpAction = NewAction; }

	UInputAction* GetLookAction() const { return LookAction; }
	void SetLookAction(UInputAction* NewAction) { LookAction = NewAction; }

	UInputAction* GetStartPlaceBlockAction() const { return StartPlaceBlockAction; }
	void SetStartPlaceBlockAction(UInputAction* NewAction) { StartPlaceBlockAction = NewAction; }

	UInputAction* GetConfirmPlaceBlockAction() const { return ConfirmPlaceBlockAction; }
	void SetConfirmPlaceBlockAction(UInputAction* NewAction) { ConfirmPlaceBlockAction = NewAction; }

	UInputAction* GetBlock1Action() const { return Block1Action; }
	void SetBlock1Action(UInputAction* NewAction) { Block1Action = NewAction; }

	UInputAction* GetBlock2Action() const { return Block2Action; }
	void SetBlock2Action(UInputAction* NewAction) { Block2Action = NewAction; }

	UInputAction* GetBlock3Action() const { return Block3Action; }
	void SetBlock3Action(UInputAction* NewAction) { Block3Action = NewAction; }

	UInputAction* GetRotatePreviewBlockAction() const { return RotatePreviewBlockAction; }
	void SetRotatePreviewBlockAction(UInputAction* NewAction) { RotatePreviewBlockAction = NewAction; }

	UInputAction* GetDeleteBlockAction() const { return DeleteBlockAction; }
	void SetDeleteBlockAction(UInputAction* NewAction) { DeleteBlockAction = NewAction; }

	UInputAction* GetHoldFAction() const { return HoldFAction; }
	void SetHoldFAction(UInputAction* NewAction) { HoldFAction = NewAction; }

	UInputAction* GetLeftClickAction() const { return LeftClickAction; }
	void SetLeftClickAction(UInputAction* NewAction) { LeftClickAction = NewAction; }
};
