#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "EGameTeam.h"
#include "BrickGamePlayerController.generated.h"

class ABrickGamePlayerState;
class UInputMappingContext;
class UInputAction;
class ULobbyUserWidget;
class UInGameUserWidget;

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

	//InGame UI
	/*uproperty(editdefaultsonly, category = "ui|ingame")
	subclassof<uingameuserwidget> ingamewidgetclass;

	uproperty()
	uingameuserwidget* ingamewidget;*/

	void InitLobbyUI();
	void InitInGameUI();

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
	class UInputAction* FKeyAction;


public:
	//getters
	UInputMappingContext* GetInputMappingContext() const { return InputMappingContext; }
	UInputAction* GetMoveAction() const { return MoveAction; }
	UInputAction* GetJumpAction() const { return JumpAction; }
	UInputAction* GetLookAction() const { return LookAction; }
	UInputAction* GetStartPlaceBlockAction() const { return StartPlaceBlockAction; }
	UInputAction* GetConfirmPlaceBlockAction() const { return ConfirmPlaceBlockAction; }
	UInputAction* GetBlock1Action() const { return Block1Action; }
	UInputAction* GetBlock2Action() const { return Block2Action; }
	UInputAction* GetBlock3Action() const { return Block3Action; }
	UInputAction* GetRotatePreviewBlockAction() const { return RotatePreviewBlockAction; }
	UInputAction* GetDeleteBlockAction() const { return DeleteBlockAction; }
	UInputAction* GetFKeyAction() const { return FKeyAction; }
	//setters
	void SetInputMappingContext(UInputMappingContext* NewContext) { InputMappingContext = NewContext; }
	void SetMoveAction(UInputAction* NewAction) { MoveAction = NewAction; }
	void SetJumpAction(UInputAction* NewAction) { JumpAction = NewAction; }
	void SetLookAction(UInputAction* NewAction) { LookAction = NewAction; }
	void SetStartPlaceBlockAction(UInputAction* NewAction) { StartPlaceBlockAction = NewAction; }
	void SetConfirmPlaceBlockAction(UInputAction* NewAction) { ConfirmPlaceBlockAction = NewAction; }
	void SetBlock1Action(UInputAction* NewAction) { Block1Action = NewAction; }
	void SetBlock2Action(UInputAction* NewAction) { Block2Action = NewAction; }
	void SetBlock3Action(UInputAction* NewAction) { Block3Action = NewAction; }
	void SetRotatePreviewBlockAction(UInputAction* NewAction) { RotatePreviewBlockAction = NewAction; }
	void SetDeleteBlockAction(UInputAction* NewAction) { DeleteBlockAction = NewAction; }
	void SetFKeyAction(UInputAction* NewAction) { FKeyAction = NewAction; }
};
