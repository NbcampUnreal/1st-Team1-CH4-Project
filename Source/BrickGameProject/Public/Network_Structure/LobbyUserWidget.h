#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LobbyUserWidget.generated.h"

class UButton;
class UTextBlock;

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

	void ActivateStartButton();

protected:
	UPROPERTY(meta = (BindWidget))
	class UButton* ReadyButton;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* ReadyButtonText;

	UPROPERTY(meta = (BindWidget))
	class UButton* StartButton;
};
