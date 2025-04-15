#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LobbyUserWidget.generated.h"

class UButton;
class UTextBlock;
class UVerticalBox;
class ULobbyUserEntry;

UCLASS()
class BRICKGAMEPROJECT_API ULobbyUserWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;
	
	UFUNCTION()
	void OnClickReady();

	UFUNCTION()
	void OnClickStart();

	UFUNCTION()
	void OnClickRedTeam();

	UFUNCTION()
	void OnClickBlueTeam();

	void ActivateStartButton(bool bEnable);
	void UpdateTeamInfo();

	FTimerHandle CheckHandle;
protected:
	UPROPERTY(meta = (BindWidget))
	UButton* ReadyButton;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* ReadyButtonText;

	UPROPERTY(meta = (BindWidget))
	UButton* StartButton;

	UPROPERTY(meta = (BindWidget))
	UButton* RedTeamButton;

	UPROPERTY(meta = (BindWidget))
	UButton* BlueTeamButton;

	UPROPERTY(meta = (BindWidget))
	UVerticalBox* VerticalBox_RedTeam;

	UPROPERTY(meta = (BindWidget))
	UVerticalBox* VerticalBox_BlueTeam;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
	TSubclassOf<ULobbyUserEntry> UserEntryWidgetClass;
};
